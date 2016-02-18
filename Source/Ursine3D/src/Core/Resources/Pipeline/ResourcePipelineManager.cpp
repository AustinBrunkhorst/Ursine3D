#include "UrsinePrecompiled.h"

#include "ResourcePipelineManager.h"

#include "ResourceImporter.h"
#include "ResourceProcessor.h"

#include "ResourceFormatWriter.h"
#include "ResourceFormatConfig.h"

namespace ursine 
{ 
    namespace resources 
    { 
        namespace pipeline 
        {
            namespace
            {
                const auto kMetaFileExtension = "meta";

                // meta file serialization is handled explicitly
                const auto kMetaKeyGUID = "guid";
                const auto kMetaKeyImporter = "importer";
                const auto kMetaKeyProcessor = "processor";
                const auto kMetaKeyProcessorOptions = "options";

                const meta::Type::Set &getImporterTypes(void)
                {
                    static auto importerTypes = typeof( ResourceImporter ).GetDerivedClasses( );

                    return importerTypes;
                }
            }

            ///////////////////////////////////////////////////////////////////

            void ResourcePipelineManager::SetConfig(const ResourcePipelineConfig &config)
            {
                m_config = config;
            }

            ///////////////////////////////////////////////////////////////////

            void ResourcePipelineManager::Build(void)
            {
                if (!is_directory( m_config.resourceDirectory ))
                    create_directories( m_config.resourceDirectory );

                if (!is_directory( m_config.buildDirectory ))
                    create_directories( m_config.buildDirectory );

                registerResources( );

                m_buildWorkerThread = std::thread( &ResourcePipelineManager::buildResources, this );

                if (m_buildWorkerThread.joinable( ))
                    m_buildWorkerThread.detach( );
            }

            ///////////////////////////////////////////////////////////////////

            void ResourcePipelineManager::InvalidateResourceMeta(ResourceItem::Handle resource)
            {
                Json output = Json::object {
                    { kMetaKeyGUID, to_string( resource->m_guid ) },
                    { kMetaKeyImporter, resource->m_metaData.importer.GetName( ) },
                    { kMetaKeyProcessor, resource->m_metaData.processor.GetName( ) },
                    { kMetaKeyProcessorOptions, resource->m_metaData.processorOptions }
                };

                auto metaJsonString = output.dump( true );

                URSINE_TODO( "Make recoverable." )
                UAssert( 
                    fs::WriteAllText( resource->m_metaFileName.string( ), metaJsonString ),
                    "Unable to write to resource meta data.\nfile: %s",
                    resource->m_metaFileName.string( )
                );
            }

            ///////////////////////////////////////////////////////////////////
            // Registration
            ///////////////////////////////////////////////////////////////////

            void ResourcePipelineManager::registerResources(void)
            {
                typedef fs::recursive_directory_iterator ResourceIterator;

                try
                {
                    ResourceIterator it( m_config.resourceDirectory );
                    ResourceIterator itEnd;

                    for (; it != itEnd; ++it)
                    {
                        if (!is_directory( *it ))
                            registerResource( *it );
                    }
                }
                catch(fs::filesystem_error &e)
                {
                    UError( "Unable to iterate resource directory.\nerror: %s",
                        e.what( )
                    );
                }
            }

            ///////////////////////////////////////////////////////////////////

            void ResourcePipelineManager::registerResource(const fs::path &fileName)
            {
                auto metaFileName = fileName;

                // add the meta extension IN ADDITION to the existing extension
                metaFileName.concat( std::string( "." ) + kMetaFileExtension );

                // this resource is already configured
                if (exists( metaFileName ))
                    addExistingResource( fileName, metaFileName );
                else
                    addDefaultResource( fileName, metaFileName );
            }

            ///////////////////////////////////////////////////////////////////

            void ResourcePipelineManager::addExistingResource(
                const fs::path &fileName, 
                const fs::path &metaFileName
            )
            {
                std::string metaDataError;
                std::string metaDataString;

                URSINE_TODO( "Make all failures recoverable." );

                UAssert( fs::LoadAllText( metaFileName.string( ), metaDataString ),
                    "Unable to load resource meta file.\nfile: %s",
                    metaFileName.string( ).c_str( )
                );

                auto metaDataJson = Json::parse( metaDataString, metaDataError );

                UAssert( metaDataError.empty( ),
                    "Unable to parse meta data resource.\nfile: %s\nerror: %s",
                    metaFileName.string( ).c_str( ),
                    metaDataError.c_str( )
                );

                auto &guidObject = metaDataJson[ kMetaKeyGUID ];

                UAssert( guidObject.is_string( ),
                    "GUID expected to be string type."
                );

                // load the guid from the string
                auto guid = GUIDStringGenerator( )( guidObject.string_value( ) );
                
                auto importerName = metaDataJson[ kMetaKeyImporter ].string_value( );
                auto importerType = meta::Type::GetFromName( importerName );

                UAssert( importerType.IsValid( ),
                    "Invalid resource importer '%s'.",
                    importerName.c_str( )
                );

                UAssert( importerType.GetDynamicConstructor( ).IsValid( ),
                    "Importer '%s' does not have a default dynamic constructor.",
                    importerType.GetName( ).c_str( )
                );

                auto processorName = metaDataJson[ kMetaKeyProcessor ].string_value( );
                auto processorType = meta::Type::GetFromName( processorName );

                UAssert( processorType.IsValid( ),
                    "Invalid resource processor '%s'.",
                    processorName.c_str( )
                );

                UAssert(processorType.GetDynamicConstructor( ).IsValid( ),
                    "Processor '%s' does not have a default dynamic constructor.",
                    processorType.GetName( ).c_str( )
                );

                auto resource = allocateResource( fileName, metaFileName, guid );

                resource->m_metaData.importer = importerType;
                resource->m_metaData.processor = processorType;
                resource->m_metaData.processorOptions = metaDataJson[ kMetaKeyProcessorOptions ];
            }

            ///////////////////////////////////////////////////////////////////

            void ResourcePipelineManager::addDefaultResource(
                const fs::path &fileName, 
                const fs::path &metaFileName
            )
            {
                auto &importerTypes = getImporterTypes( );
                auto extension = fileName.extension( ).string( );

                // remove period from extension
                if (!extension.empty( ) && extension.front( ) == '.')
                    extension.erase( extension.begin( ) );

                utils::MakeLowerCase( extension );

                meta::Type importerType;
                const ResourceImporterConfig *importerConfig = nullptr;

                // find the first importer that declares this extension
                for (auto &type : importerTypes)
                {
                    auto &meta = type.GetMeta( );

                    auto *config = meta.GetProperty<ResourceImporterConfig>( );

                    if (config && config->fileExtensions.Exists( extension ))
                    {
                        importerType = type;
                        importerConfig = config;

                        break;
                    }
                }

                URSINE_TODO( "Determine default behavior." );
                // importer does not exist, ignore it.
                if (!importerType.IsValid( ))
                    return;

                UAssert( importerType.GetDynamicConstructor( ).IsValid( ),
                    "Importer '%s' does not have a default dynamic constructor.",
                    importerType.GetName( ).c_str( )
                );

                auto resource = allocateResource( fileName, metaFileName );

                resource->m_metaData.importer = importerType;

                auto processor = importerConfig->defaultProcessor;

                UAssert( processor.IsValid( ),
                    "Importer '%s' specified an invalid processor.",
                    importerType.GetName( ).c_str( )
                );

                UAssert( processor.GetDynamicConstructor( ).IsValid( ),
                    "Processor '%s' does not have a default dynamic constructor.",
                    processor.GetName( ).c_str( )
                );

                resource->m_metaData.processor = processor;

                auto &processorMeta = processor.GetMeta( );

                auto *processorConfig = processorMeta.GetProperty<ResourceProcessorConfig>( );

                // no config assumes no import options
                if (processorConfig)
                {
                    auto optionsType = processorConfig->optionsType;

                    UAssert( optionsType.IsValid( ),
                        "Processor '%s' specified invalid options type.",
                        processor.GetName( ).c_str( )
                    );

                    auto defaultOptions = optionsType.Create( );

                    // serialize with the default options
                    resource->m_metaData.processorOptions = 
                        optionsType.SerializeJson( defaultOptions );
                }
                else
                {
                    resource->m_metaData.processorOptions = Json::object { };
                }

                InvalidateResourceMeta( resource );
            }

            ///////////////////////////////////////////////////////////////////

            ResourceItem::Handle ResourcePipelineManager::allocateResource(
                const fs::path &fileName,
                const fs::path &metaFileName,
                const GUID &guid /*= GUIDGenerator( )( )*/
            )
            {
                auto resource = std::make_shared<ResourceItem>( guid );

                resource->m_fileName = fileName;
                resource->m_metaFileName = metaFileName;
                resource->m_buildFileName = getResourceBuildFile( guid );

                auto insertion = m_database.insert( { resource->m_guid, resource } );

                // This should never happen unless someone tampers with something
                UAssert( insertion.second,
                    "Resource GUID is not unique."
                );

                return resource;
            }

            ///////////////////////////////////////////////////////////////////
            // Building
            ///////////////////////////////////////////////////////////////////

            void ResourcePipelineManager::buildResources(void)
            {
                for (auto entry : m_database)
                {
                    auto &item = entry.second;
                    
                    auto shouldBuild = true;
                    /*auto sourceTime = last_write_time( item->m_fileName );

                    auto buildFileExists = !exists( item->m_buildFileName );

                    auto shouldBuild = !buildFileExists ||
                        (sourceTime > last_write_time( item->m_buildFileName ));*/
                   
                    // build if source file is newer than build file
                    if (shouldBuild)
                        buildResource( item );
                }
            }

            void ResourcePipelineManager::buildResource(ResourceItem::Handle resource)
            {
                auto &meta = resource->m_metaData;

                auto importer = ResourceImporter::Handle( 
                    meta.importer.CreateDynamic( ).GetValue<ResourceImporter*>( ) 
                );

                auto processor = ResourceProcessor::Handle( 
                    meta.processor.CreateDynamic( ).GetValue<ResourceProcessor*>( )
                );

                URSINE_TODO( "Handle exception occurred during pipeline below." );

                ResourceImportContext importContext;

                auto importData = importer->Import( resource->m_fileName, importContext );

                ResourceProcessorContext processContext;

                auto processData = processor->Process( importData, processContext );

                UAssert( processData != nullptr,
                    "Processed data was null." 
                );

                ResourceFormatWriter formatWriter( resource );

                formatWriter.Write( processData );
            }

            ///////////////////////////////////////////////////////////////////
            // Utilities
            ///////////////////////////////////////////////////////////////////

            fs::path ResourcePipelineManager::getResourceBuildFile(const GUID &guid) const
            {
                auto file = m_config.buildDirectory / to_string( guid );

                return file.replace_extension( kResourceFormatExtension );
            }
        }
    }
}