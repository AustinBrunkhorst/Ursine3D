#include "UrsinePrecompiled.h"

#include "ResourcePipelineManager.h"

#include "ResourceImporter.h"
#include "ResourceProcessor.h"

#include "ResourceFormatWriter.h"
#include "ResourceFormatConfig.h"

#include "BuiltInResourceConfig.h"

#include "ResourceBuildContext.h"

using namespace std::chrono;

namespace ursine 
{ 
    namespace
    {
        const auto kMetaFileExtension = "meta";
        const auto kPreviewFileExtension = "png";
        const auto kCacheFileExtension = "cache";

        // meta file serialization is handled explicitly
        const auto kMetaKeyGUID = "guid";
        const auto kMetaKeyImporter = "importer";
        const auto kMetaKeyProcessor = "processor";
        const auto kMetaKeyProcessorOptions = "options";
        const auto kMetaKeySyncExclude = "syncExclude";

        // cache file serialization is handled explicitly
        const auto kCacheKeyHasPreview = "hasPreview";
        const auto kCacheKeyProcessedType = "processedType";
        const auto kCacheKeyGeneratedResources = "generatedResources";

        // maximum dimensions for a build preview
        const auto kMaxPreviewDimensions = Vec2 { 128.0f, 128.0f };

        const meta::Type::Set &getImporterTypes(void)
        {
            static auto importerTypes = typeof( rp::ResourceImporter ).GetDerivedClasses( );

            return importerTypes;
        }
    }

    ///////////////////////////////////////////////////////////////////////////

    rp::ResourcePipelineManager::ResourcePipelineManager(void)
        : EventDispatcher( this )
        , m_rootDirectory( new ResourceDirectoryNode( nullptr ) )
        , m_resourceDirectoryWatch( -1 )
    {
        m_fileWatcher.watch( );
    }

    rp::ResourcePipelineManager::~ResourcePipelineManager(void)
    {
        delete m_rootDirectory;

        m_rootDirectory = nullptr;
    }

    ///////////////////////////////////////////////////////////////////////////

    const rp::ResourcePipelineConfig &rp::ResourcePipelineManager::GetConfig(void) const
    {
        return m_config;
    }

    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::SetConfig(const ResourcePipelineConfig &config)
    {
        m_config = config;
    }

    ///////////////////////////////////////////////////////////////////////////

    rp::ResourceDirectoryNode *rp::ResourcePipelineManager::GetRootResourceDirectory(void)
    {
        return m_rootDirectory;
    }

    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::Build(void)
    {
        if (!is_directory( m_config.resourceDirectory ))
            create_directories( m_config.resourceDirectory );

        if (!is_directory( m_config.tempDirectory ))
            create_directories( m_config.tempDirectory );

        if (!is_directory( m_config.buildDirectory ))
            create_directories( m_config.buildDirectory );

        m_rootDirectory->m_path = m_config.resourceDirectory;

        registerResources( m_rootDirectory, m_config.resourceDirectory );

        m_buildWorkerThread = std::thread( 
            &ResourcePipelineManager::buildResources,
            this 
        );

        if (m_buildWorkerThread.joinable( ))
            m_buildWorkerThread.detach( );
    }

    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::WatchResourceDirectory(void)
    {
        if (IsWatchingResourceDirectory( ))
            return;

        m_resourceDirectoryWatch = m_fileWatcher.addWatch( 
            m_config.resourceDirectory.string( ), 
            this, 
            true 
        );
    }

    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::StopWatchingResourceDirectory(void)
    {
        if (!IsWatchingResourceDirectory( ))
            return;

        m_fileWatcher.removeWatch( m_resourceDirectoryWatch );
    }

    ///////////////////////////////////////////////////////////////////////////

    bool rp::ResourcePipelineManager::IsWatchingResourceDirectory(void) const
    {
        return m_resourceDirectoryWatch != -1;
    }

    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::InvalidateResourceMeta(ResourceItem::Handle resource)
    {
        auto &metaData = resource->m_metaData;

        auto output = Json::object {
            { kMetaKeyGUID, to_string( resource->m_guid ) },
            { kMetaKeyImporter, metaData.importer.GetName( ) },
            { kMetaKeyProcessor, metaData.processor.GetName( ) },
            { kMetaKeyProcessorOptions, metaData.processorOptions }
        };

        if (metaData.syncExcludeExpressions.Size( ) > 0)
        {
            Json::array syncExclude;

            for (auto &expression : metaData.syncExcludeExpressions)
                syncExclude.emplace_back( expression );

            output[ kMetaKeySyncExclude ] = syncExclude;
        }

        auto metaJsonString = Json( output ).dump( true );

        URSINE_TODO( "Make recoverable." )
        UAssert( 
            fs::WriteAllText( resource->m_metaFileName.string( ), metaJsonString ),
            "Unable to write to resource meta data.\nfile: %s",
            resource->m_metaFileName.string( ).c_str( )
        );
    }

    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::InvalidateResourceCache(ResourceItem::Handle resource)
    {
        auto &cache = resource->m_buildCache;

        auto output = Json::object {
            { kCacheKeyHasPreview, cache.hasPreview },
            { kCacheKeyProcessedType, cache.processedType.GetName( ) },
        };

        Json::array generatedResources;

        for (auto &generated : cache.generatedResources)
            generatedResources.emplace_back( to_string( generated ) );

        output[ kCacheKeyGeneratedResources ] = generatedResources;

        auto cacheJsonString = Json( output ).dump( true );

        URSINE_TODO( "Make recoverable." )
        UAssert( 
            fs::WriteAllText( resource->m_buildCacheFileName.string( ), cacheJsonString ),
            "Unable to write to resource build cache.\nfile: %s",
            resource->m_buildCacheFileName.string( ).c_str( )
        );
    }

    ///////////////////////////////////////////////////////////////////////////

    rp::ResourceItem::Handle rp::ResourcePipelineManager::GetItem(const GUID &guid)
    {
        auto search = m_database.find( guid );

        if (search == m_database.end( ))
            return nullptr;

        return search->second;
    }

    rp::ResourceItem::List rp::ResourcePipelineManager::GetItemsByType(const meta::Type &type)
    {
        ResourceItem::List matched;

        for (auto &entry : m_database)
        {
            if (entry.second->m_buildCache.processedType == type)
                matched.emplace_back( entry.second );
        }

        return matched;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Registration
    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::registerResources(
        ResourceDirectoryNode *directory, 
        const fs::path &directoryName
    )
    {
        try
        {
            fs::directory_iterator it( directoryName );
            fs::directory_iterator itEnd;

            for (; it != itEnd; ++it)
            {
                auto &entry = *it;

                if (is_directory( entry ))
                {
                    if (isDirectoryResource( entry ))
                    {
                        auto resource = registerResource( entry );

                        // we skipped over this
                        if (resource != nullptr)
                            directory->m_resources.push_back( resource );
                    } 
                    else
                    {
                        auto *subDirectory = 
                            new ResourceDirectoryNode( directory, entry );

                        directory->m_subDirectories.push_back( subDirectory );

                        registerResources( subDirectory, entry );
                    }
                }
                else
                {
                    auto resource = registerResource( entry );

                    // we skipped over this
                    if (resource != nullptr)
                        directory->m_resources.push_back( resource );
                }
            }
        }
        catch (fs::filesystem_error &e)
        {
            UError( "Unable to iterate resource directory.\nerror: %s",
                e.what( )
            );
        }
    }

    ///////////////////////////////////////////////////////////////////////////

    rp::ResourceItem::Handle rp::ResourcePipelineManager::registerResource(const fs::path &fileName, bool isGenerated /*= false */)
    {
        auto metaFileName = fileName;

        // add the meta extension IN ADDITION to the existing extension
        metaFileName.concat( std::string( "." ) + kMetaFileExtension );

        // this resource is already configured
        if (exists( metaFileName ))
            return addExistingResource( fileName, metaFileName, isGenerated );

        return addDefaultResource( fileName, metaFileName );
    }

    ///////////////////////////////////////////////////////////////////////////

    rp::ResourceItem::Handle rp::ResourcePipelineManager::addExistingResource(
        const fs::path &fileName, 
        const fs::path &metaFileName,
        bool isGenerated /*= false */
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
            "Unable to parse resource meta data.\nfile: %s\nerror: %s",
            metaFileName.string( ).c_str( ),
            metaDataError.c_str( )
        );

        auto &guidObject = metaDataJson[ kMetaKeyGUID ];

        UAssert( guidObject.is_string( ),
            "GUID expected to be string type."
        );

        // load the guid from the string
        auto guid = GUIDStringGenerator( )( guidObject.string_value( ) );

        if (isGenerated)
        {
            auto search = m_database.find( guid );

            // if this item is generated and we've already registered it
            // use it rather than creating a collision with GUIDs
            if (search != m_database.end( ))
                return search->second;
        }
                
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

        UAssert( processorType.GetDynamicConstructor( ).IsValid( ),
            "Processor '%s' does not have a default dynamic constructor.",
            processorType.GetName( ).c_str( )
        );

        auto resource = allocateResource( fileName, metaFileName, guid );

        resource->m_metaData.importer = importerType;
        resource->m_metaData.processor = processorType;
        resource->m_metaData.processorOptions = metaDataJson[ kMetaKeyProcessorOptions ];

        resource->m_metaData.syncExcludeExpressions.Clear( );

        auto &syncData = metaDataJson[ kMetaKeySyncExclude ];

        if (syncData.is_array( ))
        {
            for (auto &expression : syncData.array_items( ))
            {
                resource->m_metaData.syncExcludeExpressions.Push( expression.string_value( ) );
            }
        }

        loadResourceBuildCache( resource );

        return resource;
    }

    ///////////////////////////////////////////////////////////////////////////

    rp::ResourceItem::Handle rp::ResourcePipelineManager::addDefaultResource(
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
        meta::Type processorType;

        std::tie( importerType, processorType ) = detectResourceHandlers( fileName );

        URSINE_TODO( "Determine default behavior." );
        // importer does not exist, ignore it.
        if (!importerType.IsValid( ))
            return nullptr;

        UAssert( importerType.GetDynamicConstructor( ).IsValid( ),
            "Importer '%s' does not have a default dynamic constructor.",
            importerType.GetName( ).c_str( )
        );

        UAssert( processorType.IsValid( ),
            "Importer '%s' specified an invalid processor.",
            importerType.GetName( ).c_str( )
        );

        UAssert( processorType.GetDynamicConstructor( ).IsValid( ),
            "Processor '%s' does not have a default dynamic constructor.",
            processorType.GetName( ).c_str( )
        );

        auto resource = allocateResource( fileName, metaFileName );

        resource->m_metaData.importer = importerType;
        resource->m_metaData.processor = processorType;

        auto &processorMeta = processorType.GetMeta( );

        auto *processorConfig = processorMeta.GetProperty<ResourceProcessorConfig>( );

        // no config assumes no import options
        if (processorConfig)
        {
            auto optionsType = processorConfig->optionsType;

            UAssert( optionsType.IsValid( ),
                "Processor '%s' specified invalid options type.",
                processorType.GetName( ).c_str( )
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

        return resource;
    }

    ///////////////////////////////////////////////////////////////////////////

    rp::ResourceItem::Handle rp::ResourcePipelineManager::allocateResource(
        const fs::path &fileName,
        const fs::path &metaFileName,
        const GUID &guid /*= GUIDGenerator( )( )*/
    )
    {
        auto resource = std::make_shared<ResourceItem>( this, guid );

        resource->m_fileName = fileName;
        resource->m_metaFileName = metaFileName;
        resource->m_buildFileName = getResourceBuildFile( guid );
        resource->m_buildPreviewFileName = getResourceBuildPreviewFile( guid );
        resource->m_buildCacheFileName = getResourceBuildCacheFile( guid );

        auto insertion = m_database.insert( { resource->m_guid, resource } );

        // This should never happen unless someone tampers with something
        UAssert( insertion.second,
            "Resource GUID is not unique."
        );

        return resource;
    }

    void rp::ResourcePipelineManager::insertResource(ResourceItem::Handle resource)
    {
        auto *currentNode = m_rootDirectory;

        auto relativeDirectory = fs::MakeRelativePath( 
            m_config.resourceDirectory,
            resource->m_fileName.parent_path( )
        );

        auto currentPath = m_config.resourceDirectory;

        // add all missing directory nodes if applicable
        for (auto &directory : relativeDirectory)
        {
            currentPath /= directory;

            const auto isDirectory = [&](const ResourceDirectoryNode *node)
            {
                return node->GetPath( ).stem( ) == directory;
            };

            auto &subDirs = currentNode->m_subDirectories;

            auto search = std::find_if( subDirs.begin( ), subDirs.end( ), isDirectory );

            // doesn't exist, construct it
            if (search == subDirs.end( ))
            {
                auto *newNode = new ResourceDirectoryNode( currentNode, currentPath );

                currentNode->m_subDirectories.emplace_back( newNode );

                currentNode = newNode;
            }
            // already exists, just advance the node
            else
            {
                currentNode = *search;
            }
        }

        // currentNode will end up being the node where this resource belongs
        currentNode->m_resources.emplace_back( resource );
    }

    bool rp::ResourcePipelineManager::isDirectoryResource(const fs::path &directory)
    { 
        auto handlers = detectResourceHandlers( directory );

        return handlers.first.IsValid( ) && handlers.second.IsValid( );
    }

    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::loadResourceBuildCache(ResourceItem::Handle resource)
    {
        auto cacheFileName = getResourceBuildCacheFile( resource->m_guid );

        // it's ok that the cache file doesn't exist - it just means this resource is 
        // invalidated and we'll write the cache during build
        if (!exists( cacheFileName ))
            return;

        std::string cacheError;
        std::string cacheString;

        URSINE_TODO( "Make all failures recoverable." );

        UAssert( fs::LoadAllText( cacheFileName.string( ), cacheString ),
            "Unable to load build cache file.\nfile: %s",
            cacheFileName.string( ).c_str( )
        );

        auto cacheJson = Json::parse( cacheString, cacheError );

        UAssert( cacheError.empty( ),
            "Unable to parse build cache.\nfile: %s\nerror: %s",
            cacheFileName.string( ).c_str( ),
            cacheError.c_str( )
        );

        resource->m_buildCache.hasPreview = 
            cacheJson[ kCacheKeyHasPreview ].bool_value( );

        auto processedTypeName = 
            cacheJson[ kCacheKeyProcessedType ].string_value( );

        auto processedType =
            meta::Type::GetFromName( processedTypeName );

        UAssert( processedType.IsValid( ),
            "Invalid resource processed type '%s'.\nfile: %s",
            processedTypeName.c_str( ),
            cacheFileName.string( ).c_str( )
        );

        resource->m_buildCache.processedType = processedType;

        auto generatedResourcesObj = 
            cacheJson[ kCacheKeyGeneratedResources ];

        UAssert( generatedResourcesObj.is_array( ),
            "Build cache key '%s' expected to be array type.\nfile: %s",
            cacheFileName.string( ).c_str( )
        );

        auto &generatedResources = 
            resource->m_buildCache.generatedResources;

        generatedResources.clear( );

        // convert the string values in the json array to the array of GUIDs
        for (auto &obj : generatedResourcesObj.array_items( ))
        {
            generatedResources.emplace_back(
                GUIDStringGenerator( )( obj.string_value( ) )
            );
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // Building
    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::buildResources(void)
    {
        ResourceBuildArgs buildEvent;

        std::list<ResourceBuildContext> buildContexts;

        // collect invalidated items
        for (auto &entry : m_database)
        {
            auto &item = entry.second;

            if (buildIsInvalidated( item ))
                buildContexts.emplace_back( this, item );
        }

        // we don't know off hand what resources have previews, so just assume they
        // take 30%
        static const auto previewPercentage = 0.70f;

        auto operationIndex = 0.0f;

        auto it = buildContexts.begin( );

        // build pass
        while (it != buildContexts.end( ))
        {
            ++operationIndex;

            auto &builtContext = *it;

            buildEvent.type = RP_BUILD_RESOURCE_START;
            buildEvent.resource = builtContext.resource;
            buildEvent.progress = previewPercentage * (operationIndex / buildContexts.size( ));

            auto startTime = system_clock::now( );

            Dispatch( RP_BUILD_RESOURCE_START, &buildEvent );

            buildResource( builtContext );

            addGeneratedResources( builtContext, buildContexts );

            auto duration = system_clock::now( ) - startTime;

            buildEvent.type = RP_BUILD_RESOURCE_COMPLETE;

            buildEvent.operationDuration = static_cast<int>(
                duration_cast<milliseconds>( duration ).count( ) 
            );

            Dispatch( RP_BUILD_RESOURCE_COMPLETE, &buildEvent );

            ++it;
        }

        operationIndex = 0.0f;

        // build preview pass
        for (auto &previewContext : buildContexts)
        {
            ++operationIndex;

            buildEvent.type = RP_BUILD_RESOURCE_PREVIEW_START;
            buildEvent.resource = previewContext.resource;
            buildEvent.progress = previewPercentage + (1.0f - previewPercentage) * (operationIndex / buildContexts.size( ));

            auto startTime = system_clock::now( );

            Dispatch( RP_BUILD_RESOURCE_PREVIEW_START, &buildEvent );

            buildResourcePreview( previewContext );

            // write the cache
            InvalidateResourceCache( previewContext.resource );

            auto duration = system_clock::now( ) - startTime;

            buildEvent.type = RP_BUILD_RESOURCE_PREVIEW_COMPLETE;

            buildEvent.operationDuration = static_cast<int>(
                duration_cast<milliseconds>( duration ).count( ) 
            );

            Dispatch( RP_BUILD_RESOURCE_PREVIEW_COMPLETE, &buildEvent );
        }

        Dispatch( RP_BUILD_COMPLETE, EventArgs::Empty );
    }

    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::buildResource(ResourceBuildContext &context)
    {
        auto &resource = context.resource;
        auto &meta = resource->m_metaData;

        context.importer = ResourceImporter::Handle(
            meta.importer.CreateDynamic( ).GetValue<ResourceImporter*>( ) 
        );

        context.processor = ResourceProcessor::Handle( 
            meta.processor.CreateDynamic( ).GetValue<ResourceProcessor*>( )
        );

        URSINE_TODO( "Handle exception occurred during pipeline below." );

        auto importData = context.importer->Import( context.importContext );
        auto processData = context.processor->Process( importData, context.processorContext );

        UAssert( processData != nullptr,
            "Processed data was null." 
        );

        resource->m_buildCache.hasPreview = false;
        resource->m_buildCache.processedType = processData->GetType( );

        ResourceFormatWriter formatWriter( resource );

        formatWriter.Write( processData );
    }

    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::addGeneratedResources(
        ResourceBuildContext &context, 
        std::list<ResourceBuildContext> &outGenerated
    )
    {
        // collect generated resources in all passes
        std::vector<fs::path> allGenerated;

        auto &genImport = context.importContext.generatedResources;

        allGenerated.insert( 
            allGenerated.end( ), 
            genImport.begin( ), 
            genImport.end( ) 
        );

        auto &genProcess = context.processorContext.generatedResources;

        allGenerated.insert( 
            allGenerated.end( ), 
            genProcess.begin( ), 
            genProcess.end( ) 
        );

        for (auto &path : allGenerated)
        {
            auto resource = registerResource( path, true );

            // don't care about this this resource
            if (!resource)
                continue;

            resource->m_parent = context.resource;

            insertResource( resource );

            // add this generated resource to the input resource
            context.resource->m_buildCache.generatedResources.emplace_back( 
                resource->GetGUID( )
            );

            outGenerated.emplace_back( this, resource );
        }
    }

    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::buildResourcePreview(ResourceBuildContext &context)
    {
        // determine if the processor or importer built a preview.
        // take priority with the processor
        auto previewBuilt = 
            context.processor->BuildPreview( context.processorContext, kMaxPreviewDimensions ) ||
             context.importer->BuildPreview( context.importContext, kMaxPreviewDimensions );

        context.resource->m_buildCache.hasPreview = previewBuilt;
    }

    ///////////////////////////////////////////////////////////////////////////

    bool rp::ResourcePipelineManager::buildIsInvalidated(ResourceItem::Handle resource)
    {
        auto filesExist = exists( resource->m_buildFileName );

        filesExist = filesExist && exists( resource->m_buildCacheFileName );

        // this should always exist, but just in case
        filesExist = filesExist && exists( resource->m_metaFileName);

        // if any of the following files don't exist, this build is invalidated
        if (!filesExist)
            return true;

        auto buildTime = last_write_time( resource->m_buildFileName );
        auto sourceTime = last_write_time( resource->m_fileName );

        if (sourceTime > buildTime)
            return true;

        auto metaTime = last_write_time( resource->m_metaFileName );

        if (metaTime > buildTime)
            return true;
                
        // this puppy is all up to date
        return false;
    }

    ///////////////////////////////////////////////////////////////////////////
    // File Watching
    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::handleFileAction(
        fs::WatchID watchid,
        const std::string &dir,
        const std::string &filename,
        fs::Action action,
        std::string oldFilename
        )
    {
        const char *names[] = { "Invalid", "Add", "Delete", "Modified", "Moved" };

        std::cout << "=================" << std::endl;
        std::cout << "dir: " << dir << std::endl;
        std::cout << "filename: " << filename << std::endl;
        std::cout << "action: " << names[ action ] << std::endl;
        std::cout << "oldFileName: " << oldFilename << std::endl;

        std::cout << std::endl;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Utilities
    ///////////////////////////////////////////////////////////////////////////

    fs::path rp::ResourcePipelineManager::getResourceBuildFile(const GUID &guid) const
    {
        auto file = m_config.buildDirectory / to_string( guid );

        return file.replace_extension( kResourceFormatExtension );
    }

    ///////////////////////////////////////////////////////////////////////////

    fs::path rp::ResourcePipelineManager::getResourceBuildPreviewFile(const GUID &guid) const
    {
        auto file = m_config.buildDirectory / to_string( guid );

        return file.replace_extension( kPreviewFileExtension );
    }

    ///////////////////////////////////////////////////////////////////////////

    fs::path rp::ResourcePipelineManager::getResourceBuildCacheFile(const GUID &guid) const
    {
        auto file = m_config.buildDirectory / to_string( guid );

        return file.replace_extension( kCacheFileExtension );
    }

    rp::TypePair rp::ResourcePipelineManager::detectResourceHandlers(const fs::path &path) const
    {
        auto &importerTypes = getImporterTypes( );
        auto extension = path.extension( ).string( );

        // remove period from extension
        if (!extension.empty( ) && extension.front( ) == '.')
            extension.erase( extension.begin( ) );

        utils::MakeLowerCase( extension );

        auto &builtInHandlers = GetBuiltInResourceHandlers( );

        auto handlerSearch = builtInHandlers.find( extension );

        // this extension is a built in resource, use the explicitly defined
        // importer and processor
        if (handlerSearch != builtInHandlers.end( ))
            return handlerSearch->second;

        TypePair handlers;

        const ResourceImporterConfig *importerConfig = nullptr;

        // find the first importer that declares this extension
        for (auto &type : importerTypes)
        {
            auto &meta = type.GetMeta( );

            auto *config = meta.GetProperty<ResourceImporterConfig>( );

            if (config && config->fileExtensions.Exists( extension ))
            {
                handlers.first = type;
                importerConfig = config;

                break;
            }
        }

        if (importerConfig)
            handlers.second = importerConfig->defaultProcessor;

        return handlers;
    }
}