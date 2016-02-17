#include "UrsinePrecompiled.h"

#include "ResourcePipelineManager.h"

#include "DefaultImporter.h"

namespace ursine 
{ 
    namespace resources 
    { 
        namespace pipeline 
        {
            namespace
            {
                const auto kMetaFileExtension = ".meta";

                const meta::Type::Set &getImporterTypes(void)
                {
                    static auto importerTypes = typeof( ResourceImporter ).GetDerivedClasses( );

                    return importerTypes;
                }
            }

            void ResourcePipelineManager::SetConfig(const ResourcePipelineConfig &config)
            {
                m_config = config;
            }

            void ResourcePipelineManager::Build(void)
            {
                if (!is_directory( m_config.resourceDirectory ))
                    create_directories( m_config.resourceDirectory );

                if (!is_directory( m_config.buildDirectory ))
                    create_directories( m_config.buildDirectory );

                registerResources( );
            }

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

            void ResourcePipelineManager::registerResource(const fs::path &filename)
            {
                auto metaFileName = filename;

                // add the meta extension
                metaFileName.concat( kMetaFileExtension );

                Json metaDataJson;

                // this resource is already configured
                if (exists( metaFileName ))
                {
                    std::string metaDataError;
                    std::string metaDataString;

                    URSINE_TODO( "Make all failures recoverable." );

                    UAssert( fs::LoadAllText( metaFileName.string( ), metaDataString ),
                        "Unable to load resource meta file.\nfile: %s",
                        metaFileName.string( ).c_str( )
                    );

                    metaDataJson = Json::parse( metaDataString, metaDataError );

                    UAssert( metaDataError.empty( ),
                        "Unable to parse meta data resource.\nfile: %s\nerror: %s",
                        metaFileName.string( ).c_str( ),
                        metaDataError.c_str( )
                    );
                }
                else
                {
                    metaDataJson = createDefaultResourceMetaData( filename );
                }

                initializeResource( filename, metaDataJson );
            }

            void ResourcePipelineManager::initializeResource(const fs::path &filename, const Json &metaData)
            {
                
            }

            Json ResourcePipelineManager::createDefaultResourceMetaData(const fs::path &filename)
            {
                auto &importerTypes = getImporterTypes( );
                auto extension = filename.extension( ).string( );
                
                utils::MakeLowerCase( extension );

                // default to the default importer
                auto importerType = typeof( DefaultImporter );

                // find the first importer that declares this extension
                for (auto &type : importerTypes)
                {
                    auto &meta = type.GetMeta( );

                    auto *config = meta.GetProperty<ResourceImporterConfig>( );

                    if (config && config->fileExtensions.Exists( extension ))
                    {
                        importerType = type;

                        break;
                    }
                }

                Json::object meta;

                meta[ "importer" ] = importerType.GetName( );

                return meta;
            }
        }
    }
}