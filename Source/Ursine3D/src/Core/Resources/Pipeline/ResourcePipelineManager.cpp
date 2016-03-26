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
        const auto kMetaFileExtension = ".meta";
        const auto kPreviewFileExtension = ".png";
        const auto kCacheFileExtension = ".cache";

        // meta file serialization is handled explicitly
        const auto kMetaKeyGUID = "guid";
        const auto kMetaKeyImporter = "importer";
        const auto kMetaKeyProcessor = "processor";
        const auto kMetaKeyProcessorOptions = "options";
        const auto kMetaKeySyncExclude = "syncExclude";

        // cache file serialization is handled explicitly
        const auto kCacheKeyHasPreview = "hasPreview";
        const auto kCacheKeyParent = "parent";
        const auto kCacheKeyProcessedType = "processedType";
        const auto kCacheKeyGeneratedResources = "generatedResources";

        // maximum dimensions for a build preview
        const auto kMaxPreviewDimensions = Vec2 { 128.0f, 128.0f };

        // duration to wait until there are no actions on pending file action
        // to be considered stale
        const auto kResourceActionStaleDuration = milliseconds( 1500 );

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
        , m_isProcessingFileActions( false )
        , m_resourceDirectoryWatch( -1 )
    {
        
    }

    rp::ResourcePipelineManager::~ResourcePipelineManager(void)
    {
        StopWatchingResourceDirectory( );

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

        m_fileWatcher.watch( );

        m_isProcessingFileActions = true;

        m_fileActionProcessorThread = std::thread(
            &ResourcePipelineManager::processPendingFileActions,
            this 
        );

        if (m_fileActionProcessorThread.joinable( ))
            m_fileActionProcessorThread.detach( );
    }

    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::StopWatchingResourceDirectory(void)
    {
        if (!IsWatchingResourceDirectory( ))
            return;

        m_isProcessingFileActions = false;

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

        UAssertCatchable( 
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

        // assign the parent if it exists
        output[ kCacheKeyParent ] = to_string( resource->m_buildCache.parent );

        Json::array generatedResources;

        for (auto &generated : cache.generatedResources)
            generatedResources.emplace_back( to_string( generated ) );

        output[ kCacheKeyGeneratedResources ] = generatedResources;

        auto cacheJsonString = Json( output ).dump( true );

        UAssertCatchable( 
            fs::WriteAllText( resource->m_buildCacheFileName.string( ), cacheJsonString ),
            "Unable to write to resource build cache.\nfile: %s",
            resource->m_buildCacheFileName.string( ).c_str( )
        );
    }

    ///////////////////////////////////////////////////////////////////////////

    rp::ResourceItem::Handle rp::ResourcePipelineManager::GetItem(const GUID &guid) const
    {
        auto search = m_database.find( guid );

        return search == m_database.end( ) ? nullptr : search->second;
    }

    ///////////////////////////////////////////////////////////////////////////

    rp::ResourceItem::Handle rp::ResourcePipelineManager::GetItem(const fs::path &sourceFile) const
    {
        auto search = m_pathToResource.find( sourceFile );

        return search == m_pathToResource.end( ) ? nullptr : search->second;
    }

    ///////////////////////////////////////////////////////////////////////////

    rp::ResourceItem::List rp::ResourcePipelineManager::GetItemsByType(const meta::Type &type) const
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

    void rp::ResourcePipelineManager::RemoveItem(ResourceItem::Handle item)
    {
        ResourceChangeArgs e;

        e.type = RP_RESOURCE_REMOVED;
        e.resource = item;

        Dispatch( e.type, &e );

        removeResource( item, true );
    }

    ///////////////////////////////////////////////////////////////////////////

    fs::path rp::ResourcePipelineManager::CreateTemporaryFileName(void) const
    {
        auto guid = GUIDGenerator( )( );

        return m_config.tempDirectory / to_string( guid );
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
            fs::DirectoryIterator it( directoryName );
            fs::DirectoryIterator itEnd;

            for (; it != itEnd; ++it)
            {
                auto &entry = it->path( );

                if (is_directory( entry ))
                {
                    // treat as directory resource
                    if (hasResourceHandlers( entry ))
                    {
                        ResourceItem::Handle resource;

                        try
                        {
                            resource = registerResource( entry );
                        }
                        catch (AssertionException &e)
                        {
                            URSINE_UNUSED( e );

                            UWarning( "Unable to register resource.\nresource: %s\nerror: %s", 
                                entry.string( ).c_str( ),
                                e.GetErrorMessage( ).c_str( )
                            );

                            resource = nullptr;
                        }

                        // we skipped over this
                        if (resource != nullptr)
                        {
                            directory->m_resources.push_back( resource );

                            resource->m_isDirectoryResource = true;
                            resource->m_directoryNode = directory;
                        }
                    } 
                    // treat as directory
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
                    ResourceItem::Handle resource;

                    try
                    {
                        resource = registerResource( entry );
                    }
                    catch (AssertionException &e)
                    {
                        URSINE_UNUSED( e );

                        UWarning( "Unable to register resource.\nresource: %s\nerror: %s", 
                            entry.string( ).c_str( ),
                            e.GetErrorMessage( ).c_str( )
                        );

                        resource = nullptr;
                    }

                    // we skipped over this
                    if (resource != nullptr)
                    {
                         directory->m_resources.push_back( resource ); 

                         resource->m_directoryNode = directory;
                    }
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

    rp::ResourceItem::Handle rp::ResourcePipelineManager::registerResource(
        const fs::path &fileName
    )
    {
        // ignore meta files
        if (fileName.extension( ) == kMetaFileExtension)
            return nullptr;
        
        auto metaFileName = getResourceMetaPath( fileName );

        // this resource is already configured
        if (exists( metaFileName ))
            return addExistingResource( fileName );

        return addDefaultResource( fileName );
    }

    ///////////////////////////////////////////////////////////////////////////

    rp::ResourceItem::Handle rp::ResourcePipelineManager::addExistingResource(
        const fs::path &fileName
    )
    {
        auto metaFileName = getResourceMetaPath( fileName );

        std::string metaDataError;
        std::string metaDataString;

        UAssertCatchable( fs::LoadAllText( metaFileName.string( ), metaDataString ),
            "Unable to load resource meta file.\nfile: %s",
            metaFileName.string( ).c_str( )
        );

        auto metaDataJson = Json::parse( metaDataString, metaDataError );

        UAssertCatchable( metaDataError.empty( ),
            "Unable to parse resource meta data.\nfile: %s\nerror: %s",
            metaFileName.string( ).c_str( ),
            metaDataError.c_str( )
        );

        auto &guidObject = metaDataJson[ kMetaKeyGUID ];

        UAssertCatchable( guidObject.is_string( ),
            "GUID expected to be string type."
        );

        GUID guid;

        try
        {
            // load the guid from the string
            guid = GUIDStringGenerator( )( guidObject.string_value( ) );
        }
        catch (...)
        {
            UAssertCatchable( false,
                "Unable to parse GUID in meta file."    
            );
        }
                
        auto importerName = metaDataJson[ kMetaKeyImporter ].string_value( );
        auto importerType = meta::Type::GetFromName( importerName );

        UAssertCatchable( importerType.IsValid( ),
            "Invalid resource importer '%s'.",
            importerName.c_str( )
        );

        UAssertCatchable( importerType.GetDynamicConstructor( ).IsValid( ),
            "Importer '%s' does not have a default dynamic constructor.",
            importerType.GetName( ).c_str( )
        );

        auto processorName = metaDataJson[ kMetaKeyProcessor ].string_value( );
        auto processorType = meta::Type::GetFromName( processorName );

        UAssertCatchable( processorType.IsValid( ),
            "Invalid resource processor '%s'.",
            processorName.c_str( )
        );

        UAssertCatchable( processorType.GetDynamicConstructor( ).IsValid( ),
            "Processor '%s' does not have a default dynamic constructor.",
            processorType.GetName( ).c_str( )
        );

        ResourceItem::Handle resource = nullptr;

        try
        {
            resource = allocateResource( fileName, guid );
        }
        catch (AssertionException &e)
        {
            URSINE_UNUSED( e );

            UWarning( "Unable to allocate resource.\nresource: %s\nerror: %s", 
                fileName.string( ).c_str( ),
                e.GetErrorMessage( ).c_str( )
            );

            return nullptr;
        }
        
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

        try
        {
            loadResourceBuildCache( resource );
        }
        catch (AssertionException &e)
        {
            URSINE_UNUSED( e );

            UWarning( "Unable to load resource cache.\nresource: %s\nerror: %s", 
                fileName.string( ).c_str( ),
                e.GetErrorMessage( ).c_str( )
            );

            removeResource( resource );

            return nullptr;
        }

        return resource;
    }

    ///////////////////////////////////////////////////////////////////////////

    rp::ResourceItem::Handle rp::ResourcePipelineManager::addDefaultResource(
        const fs::path &fileName
    )
    {
        auto meta = createDefaultResourceMeta( fileName );

        // doesn't have an importer
        if (!meta.importer.IsValid( ))
            return nullptr;

        ResourceItem::Handle resource;

        try
        {
            resource = allocateResource( fileName );
        }
        catch (AssertionException &e)
        {
            URSINE_UNUSED( e );

            UWarning( "Unable to allocate resource.\nresource: %s\nerror: %s", 
                fileName.string( ).c_str( ),
                e.GetErrorMessage( ).c_str( )
            );

            return nullptr;
        }

        resource->m_metaData = meta;

        try
        {
            InvalidateResourceMeta( resource );
        }
        catch (AssertionException &e)
        {
            URSINE_UNUSED( e );

            UWarning( "Unable to invalidate meta.\nresource: %s\nerror: %s", 
                fileName.string( ).c_str( ),
                e.GetErrorMessage( ).c_str( )
            );
        }

        return resource;
    }

    ///////////////////////////////////////////////////////////////////////////

    rp::ResourceItem::Handle rp::ResourcePipelineManager::allocateResource(
        const fs::path &fileName,
        const GUID &guid /*= GUIDGenerator( )( )*/
    )
    {
        auto resource = std::make_shared<ResourceItem>( this, guid );

        resource->m_fileName = fileName;
        resource->m_metaFileName = getResourceMetaPath( fileName );
        resource->m_buildFileName = getResourceBuildPath( guid );
        resource->m_buildPreviewFileName = getResourceBuildPreviewPath( guid );
        resource->m_buildCacheFileName = getResourceBuildCachePath( guid );

        auto insertion = m_database.insert( { resource->m_guid, resource } );

        // this should never happen unless someone tampers with something
        UAssertCatchable( insertion.second,
            "Resource GUID is not unique."
        );

        m_pathToResource[ fileName ] = resource;

        return resource;
    }

    ///////////////////////////////////////////////////////////////////////////

    rp::ResourceItem::Handle rp::ResourcePipelineManager::allocateGeneratedResource(
        ResourceItem::Handle parent, 
        const fs::path &sourceFile
    )
    {
        auto search = m_pathToResource.find( sourceFile );

        // if the source file already exists, return the respective resource item
        if (search != m_pathToResource.end( ))
            return search->second;

        auto meta = createDefaultResourceMeta( sourceFile );

        auto generated = allocateResource( sourceFile );

        generated->m_metaData = meta;

        if (parent)
        {
            generated->m_buildCache.parent = parent->m_guid;

            parent->m_buildCache.generatedResources.emplace( generated->m_guid );
        }

        insertResource( generated );

        InvalidateResourceMeta( generated );

        return generated;
    }

    ///////////////////////////////////////////////////////////////////////////

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

        // set the owning directory node
        resource->m_directoryNode = currentNode;
    }

    ///////////////////////////////////////////////////////////////////////////

    bool rp::ResourcePipelineManager::hasResourceHandlers(const fs::path &directory)
    { 
        auto handlers = detectResourceHandlers( directory );

        return handlers.first.IsValid( ) && handlers.second.IsValid( );
    }

    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::reloadResourceMeta(ResourceItem::Handle resource)
    {
        std::string metaDataError;
        std::string metaDataString;

        auto &metaFileName = resource->m_metaFileName;

        UAssertCatchable( fs::LoadAllText( metaFileName.string( ), metaDataString ),
            "Unable to load resource meta file.\nfile: %s",
            metaFileName.string( ).c_str( )
        );

        auto metaDataJson = Json::parse( metaDataString, metaDataError );

        UAssertCatchable( metaDataError.empty( ),
            "Unable to parse resource meta data.\nfile: %s\nerror: %s",
            metaFileName.string( ).c_str( ),
            metaDataError.c_str( )
        );

        auto &guidObject = metaDataJson[ kMetaKeyGUID ];

        UAssertCatchable( guidObject.is_string( ),
            "GUID expected to be string type."
        );
        
        auto importerName = metaDataJson[ kMetaKeyImporter ].string_value( );
        auto importerType = meta::Type::GetFromName( importerName );

        UAssertCatchable( importerType.IsValid( ),
            "Invalid resource importer '%s'.",
            importerName.c_str( )
        );

        UAssertCatchable( importerType.GetDynamicConstructor( ).IsValid( ),
            "Importer '%s' does not have a default dynamic constructor.",
            importerType.GetName( ).c_str( )
        );

        auto processorName = metaDataJson[ kMetaKeyProcessor ].string_value( );
        auto processorType = meta::Type::GetFromName( processorName );

        UAssertCatchable( processorType.IsValid( ),
            "Invalid resource processor '%s'.",
            processorName.c_str( )
        );

        UAssertCatchable( processorType.GetDynamicConstructor( ).IsValid( ),
            "Processor '%s' does not have a default dynamic constructor.",
            processorType.GetName( ).c_str( )
        );
        
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
    }

    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::loadResourceBuildCache(ResourceItem::Handle resource)
    {
        auto cacheFileName = getResourceBuildCachePath( resource->m_guid );

        // it's ok that the cache file doesn't exist - it just means this resource is 
        // invalidated and we'll write the cache during build
        if (!exists( cacheFileName ))
            return;

        std::string cacheError;
        std::string cacheString;

        UAssertCatchable( fs::LoadAllText( cacheFileName.string( ), cacheString ),
            "Unable to load build cache file.\nfile: %s",
            cacheFileName.string( ).c_str( )
        );

        auto cacheJson = Json::parse( cacheString, cacheError );

        UAssertCatchable( cacheError.empty( ),
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

        UAssertCatchable( processedType.IsValid( ),
            "Invalid resource processed type '%s'.\nfile: %s",
            processedTypeName.c_str( ),
            cacheFileName.string( ).c_str( )
        );

        resource->m_buildCache.processedType = processedType;

        auto generatedResourcesObj = 
            cacheJson[ kCacheKeyGeneratedResources ];

        UAssertCatchable( generatedResourcesObj.is_array( ),
            "Build cache key '%s' expected to be array type.\nfile: %s",
            cacheFileName.string( ).c_str( )
        );

        auto &generatedResources = 
            resource->m_buildCache.generatedResources;

        generatedResources.clear( );

        // convert the string values in the json array to the array of GUIDs
        for (auto &obj : generatedResourcesObj.array_items( ))
        {
            generatedResources.emplace( 
                GUIDStringGenerator( )( obj.string_value( ) ) 
            );
        }

        auto &parentObj = cacheJson[ kCacheKeyParent ];

        // attempt to set the parent with the cached GUID
        if (parentObj.is_string( ))
        {
            resource->m_buildCache.parent =
                GUIDStringGenerator( )( parentObj.string_value( ) );
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // Building
    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::buildResources(void)
    {
        ResourceBuildArgs buildEvent;

        std::list<ResourceBuildContext> buildContexts;
        std::vector<ResourceBuildContext*> failedContexts;

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

            auto &buildContext = *it;

            buildEvent.type = RP_BUILD_RESOURCE_START;
            buildEvent.resource = buildContext.resource;
            buildEvent.progress = previewPercentage * (operationIndex / buildContexts.size( ));

            auto startTime = system_clock::now( );

            Dispatch( buildEvent.type, &buildEvent );

            buildEvent.successful = true;

            try
            {
                buildResource( buildContext );
                addGeneratedResources( buildContext, buildContexts );
            }
            catch (AssertionException &e)
            {
                UWarning( 
                    "Error building resource.\n"
                    "resource: %s\n"
                    "error: %s\n"
                    "from: %s\n"
                    "in: %s\n"
                    "on line: %i", 
                    buildContext.resource->GetSourceFileName( ).string( ).c_str( ),
                    e.GetErrorMessage( ).c_str( ),
                    e.GetFunction( ).c_str( ),
                    e.GetFile( ).c_str( ),
                    e.GetLine( )
                );

                buildEvent.error = e;
                buildEvent.successful = false;

                failedContexts.emplace_back( &buildContext );
            }

            auto duration = system_clock::now( ) - startTime;

            buildEvent.type = RP_BUILD_RESOURCE_COMPLETE;

            buildEvent.operationDuration = static_cast<int>(
                duration_cast<milliseconds>( duration ).count( ) 
            );

            Dispatch( buildEvent.type, &buildEvent );

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

            Dispatch( buildEvent.type, &buildEvent );

            buildEvent.successful = true;

            try
            {
                buildResourcePreview( previewContext );

                // write the cache
                InvalidateResourceCache( previewContext.resource );
            }
            catch (AssertionException &e)
            {
                UWarning( 
                    "Error building resource preview.\n"
                    "resource: %s\n"
                    "error: %s\n"
                    "from: %s\n"
                    "in: %s\n"
                    "on line: %i", 
                    previewContext.resource->GetSourceFileName( ).string( ).c_str( ),
                    e.GetErrorMessage( ).c_str( ),
                    e.GetFunction( ).c_str( ),
                    e.GetFile( ).c_str( ),
                    e.GetLine( )
                );

                buildEvent.error = e;
                buildEvent.successful = false;
            }

            auto duration = system_clock::now( ) - startTime;

            buildEvent.type = RP_BUILD_RESOURCE_PREVIEW_COMPLETE;

            buildEvent.operationDuration = static_cast<int>(
                duration_cast<milliseconds>( duration ).count( ) 
            );

            Dispatch( buildEvent.type, &buildEvent );
        }

        for (auto *failedBuild : failedContexts)
            removeResource( failedBuild->resource );

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

        auto importData = context.importer->Import( context.importContext );
        auto processData = context.processor->Process( importData, context.processorContext );

        UAssertCatchable( processData != nullptr,
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
        auto &genImport = context.importContext.generatedResources;

        for (auto &generated : genImport)
            outGenerated.emplace_back( this, generated );

        auto &genProcess = context.processorContext.generatedResources;

        for (auto &generated : genProcess)
            outGenerated.emplace_back( this, generated );
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
        filesExist = filesExist && exists( resource->m_metaFileName );

        // if any of the following files don't exist, this build is invalidated
        if (!filesExist)
            return true;

        auto buildTime = last_write_time( resource->m_buildFileName );
        auto metaTime = last_write_time( resource->m_metaFileName );

        if (metaTime > buildTime)
            return true;

        // directory resources are invalidated if recursively, any files
        // are newer than the build time
        if (resource->m_isDirectoryResource)
        {
            fs::RecursiveDirectoryIterator it( resource->GetSourceFileName( ) );
            fs::RecursiveDirectoryIterator itEnd;

            auto *syncConfig = resource->m_metaData.importer.GetMeta( ).GetProperty<ResourceSyncConfig>( );

            for (; it != itEnd; ++it)
            {
                auto &directoryItem = *it;
                
                // skip checking this entry if it is explicitly excluded from syncing
                if (syncConfig && syncConfig->IsExcluded( directoryItem.path( ).string( ) ))
                    continue;

                auto fileTime = last_write_time( directoryItem );

                if (fileTime > buildTime)
                    return true;
            }
        }
        else
        {
            auto sourceTime = last_write_time( resource->m_fileName );

            if (sourceTime > buildTime)
                return true;
        }

        // this puppy is all up to date
        return false;
    }

    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::rebuildResource(ResourceItem::Handle resource)
    {
        ResourceBuildContext buildContext( this, resource );

        buildResource( buildContext );
        buildResourcePreview( buildContext );

        URSINE_TODO( "Make generated resources better" );

        InvalidateResourceCache( resource );
    }

    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::removeResource(ResourceItem::Handle resource, bool deleteFiles /*= false */)
    {
        auto parent = resource->GetParent( );

        // remove this from its parent generated resources
        if (parent)
            parent->m_buildCache.generatedResources.erase( resource->m_guid );

        auto *directoryNode = resource->m_directoryNode;

        // has it been inserted yet?
        if (directoryNode)
        {
            auto search = std::find( 
                directoryNode->m_resources.begin( ), 
                directoryNode->m_resources.end( ),
                resource 
            );

            // remove it from the node
            if (search != directoryNode->m_resources.end( ))
                directoryNode->m_resources.erase( search );
        }

        m_database.erase( resource->m_guid );

        m_pathToResource.erase( resource->m_fileName );

        if (deleteFiles)
        {
            try
            {
                remove( resource->m_fileName );
                remove( resource->m_metaFileName );
                remove( resource->m_buildFileName );
                remove( resource->m_buildPreviewFileName );
                remove( resource->m_buildCacheFileName );
            }
            catch (...)
            {
                URSINE_TODO( "Determine behavior" );
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // File Watching
    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::handleFileAction(
        fs::WatchID id,
        const std::string &directory,
        const std::string &fileName,
        fs::Action type,
        std::string oldFileName
    )
    {
        FileWatchAction actionData;

        actionData.type = type;
        actionData.directory = directory;
        actionData.fileName = fileName;
        actionData.oldFileName = oldFileName;
        actionData.absoluteFilePath = actionData.directory / actionData.fileName;

        actionData.time = system_clock::now( );

        std::lock_guard<std::mutex> lock( m_fileWatchActionMutex );

        auto directoryResource = getDirectoryResource( actionData.absoluteFilePath );

        // ignore deletions in directory resources -- almost all implementations of 
        // file editors create temporary files, so we can't use this action
        // meaningfully - unless it's the ACTUAL directory itself
        if (directoryResource && 
            type == fs::Action::Delete && 
            actionData.absoluteFilePath != directoryResource->m_fileName
        )
        {
            return;
        }

        // if this file is contained in a directory resource, make sure the action is mapped
        // to the directory, not the modified subresource itself
        m_pendingFileActions[
            directoryResource ? 
                directoryResource->m_fileName : 
                actionData.absoluteFilePath
        ] = actionData;
    }

    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::processPendingFileActions(void)
    {
        while (m_isProcessingFileActions)
        {
            decltype( m_pendingFileActions ) actionsCopy;

            // clear the pending file actions by move constructing it to the copy
            {
                std::lock_guard<std::mutex> lock( m_fileWatchActionMutex );

                actionsCopy = move( m_pendingFileActions );
            }

            for (auto &entry : actionsCopy)
                processPendingFileAction( entry.first, entry.second );

            // make sure we don't exhaust the CPU
            std::this_thread::sleep_for( milliseconds( 500 ) );
        }
    }

    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::processPendingFileAction(const fs::path &resourceFile, const FileWatchAction &action)
    {
        auto now = system_clock::now( );

        // this guy isn't stale yet, ignore him
        if (now - action.time > kResourceActionStaleDuration)
            return;

        switch (action.type)
        {
        case efsw::Actions::Add:
            std::cout << "DIR (" << action.directory << ") FILE (" << action.fileName << ") has event Added" << std::endl;
            break;
        case efsw::Actions::Delete:
            std::cout << "DIR (" << action.directory << ") FILE (" << action.fileName << ") has event Delete" << std::endl;
            break;
        case efsw::Actions::Modified:
            std::cout << "DIR (" << action.directory << ") FILE (" << action.fileName << ") has event Modified" << std::endl;
            break;
        case efsw::Actions::Moved:
            std::cout << "DIR (" << action.directory << ") FILE (" << action.fileName << ") has event Moved from (" << action.oldFileName << ")" << std::endl;
            break;
        default:
            std::cout << "Should never happen!" << std::endl;
        }

        switch (action.type)
        {
        case fs::Actions::Delete:
        {
            URSINE_TODO( "Resource deletion" );
        }
        break;
        case fs::Actions::Moved:
        case fs::Actions::Modified:
        {
            auto extension = resourceFile.extension( ).string( );

            utils::MakeLowerCase( extension );

            // meta file modified
            if (extension == kMetaFileExtension)
            {
                auto sourceFile = change_extension( resourceFile, "" );

                auto search = m_pathToResource.find( sourceFile );

                // this resource doesn't exist
                if (search == m_pathToResource.end( ))
                    return;

                // modify the resource this meta file represents
                auto handler = std::thread( &ResourcePipelineManager::onResourceModified, this, search->second );
                
                if (handler.joinable( ))
                    handler.detach( );

                return;
            }

            auto search = m_pathToResource.find( resourceFile );

            // if it doesn't exist, we're assuming it is added
            if (search == m_pathToResource.end( ))
            {
                auto handler = std::thread( &ResourcePipelineManager::onResourceAdded, this, resourceFile );
            
                if (handler.joinable( ))
                    handler.detach( );
            }
            // exists, so we'll modify it (if it's actually invalidated)
            else if (buildIsInvalidated( search->second ))
            {
                auto handler = std::thread( &ResourcePipelineManager::onResourceModified, this, search->second );
                
                if (handler.joinable( ))
                    handler.detach( );
            }
        }
        break;
        default:
            break;
        }
    }

    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::onResourceAdded(const fs::path &fileName)
    {
        std::lock_guard<std::mutex> lock( m_buildMutex );

        ResourceItem::Handle resource;

        try
        {
            resource = registerResource( fileName );
        }
        catch (AssertionException &e)
        {
            // if UWarnings are disabled, tell the compiler to hush
            URSINE_UNUSED( e );

            UWarning( "Unable to add resource.\nresource: %s\nerror: %s", 
                fileName.string( ).c_str( ),
                e.GetErrorMessage( ).c_str( )
            );

            return;
        }

        // we weren't interested in this or something borked
        if (!resource)
            return;

        try
        {
            insertResource( resource );
            rebuildResource( resource );
        }
        catch (AssertionException &e)
        {
            URSINE_UNUSED( e );

            UWarning( 
                "Error adding resource.\n"
                "resource: %s\n"
                "error: %s\n"
                "from: %s\n"
                "in: %s\n"
                "on line: %i", 
                fileName.string( ).c_str( ),
                e.GetErrorMessage( ).c_str( ),
                e.GetFunction( ).c_str( ),
                e.GetFile( ).c_str( ),
                e.GetLine( )
            );

            return;
        }

        resource->m_isDirectoryResource = 
            is_directory( fileName ) && hasResourceHandlers( fileName );

        Application::PostMainThread( [=]
        {
            ResourceChangeArgs e;

            e.type = RP_RESOURCE_ADDED;
            e.resource = resource;

            Dispatch( RP_RESOURCE_ADDED, &e );
        } );
    }

    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::onResourceModified(ResourceItem::Handle resource)
    {
        std::lock_guard<std::mutex> lock( m_buildMutex );

        try
        {
            auto metaTime = last_write_time( resource->m_metaFileName );
            auto sourceTime = last_write_time( resource->m_fileName );

            // reload meta if it's invalidated
            if (metaTime > sourceTime)
                reloadResourceMeta( resource );

            rebuildResource( resource );
        }
        catch (AssertionException &e)
        {
            URSINE_UNUSED( e );

            UWarning( 
                "Error rebuilding resource.\n"
                "resource: %s\n"
                "error: %s\n"
                "from: %s\n"
                "in: %s\n"
                "on line: %i", 
                resource->GetSourceFileName( ).string( ).c_str( ),
                e.GetErrorMessage( ).c_str( ),
                e.GetFunction( ).c_str( ),
                e.GetFile( ).c_str( ),
                e.GetLine( )
            );

            return;
        }
        catch (...)
        {
            UWarning( "Unexpected exception while reloading resource.\nresource: %s", 
                resource->GetSourceFileName( ).string( ).c_str( )
            );
        }

        std::cout << "modified: " << resource->GetSourceFileName( ) << std::endl;

        Application::PostMainThread( [=]
        {
            ResourceChangeArgs e;

            e.type = RP_RESOURCE_MODIFIED;
            e.resource = resource;

            Dispatch( RP_RESOURCE_MODIFIED, &e );
        } );
    }

    ///////////////////////////////////////////////////////////////////////////

    void rp::ResourcePipelineManager::onResourceRemoved(ResourceItem::Handle resource)
    {
        
    }

    
    ///////////////////////////////////////////////////////////////////////////
    // Utilities
    ///////////////////////////////////////////////////////////////////////////

    fs::path rp::ResourcePipelineManager::getResourceMetaPath(const fs::path &fileName) const
    {
        auto metaPath = fileName;

        metaPath.concat( kMetaFileExtension );

        return metaPath;
    }

    ///////////////////////////////////////////////////////////////////////////

    rp::ResourceItem::Handle rp::ResourcePipelineManager::getDirectoryResource(const fs::path &fileName) const
    {
        std::lock_guard<std::mutex> lock( m_databaseMutex );

        // search for a directory resource where the directory contains this file name
        for (auto &entry : m_database)
        {
            auto &item = entry.second;

            if (item->m_isDirectoryResource && 
                fs::PathContainsFile( item->m_fileName, fileName )
            )
            {
                return item;
            }  
        }

        return nullptr;
    }

    ///////////////////////////////////////////////////////////////////////////

    fs::path rp::ResourcePipelineManager::getResourceBuildPath(const GUID &guid) const
    {
        return change_extension( 
            m_config.buildDirectory / to_string( guid ),
            kResourceFormatExtension 
        );
    }

    ///////////////////////////////////////////////////////////////////////////

    fs::path rp::ResourcePipelineManager::getResourceBuildPreviewPath(const GUID &guid) const
    {
        return change_extension( 
            m_config.buildDirectory / to_string( guid ),
            kPreviewFileExtension 
        );
    }

    ///////////////////////////////////////////////////////////////////////////

    fs::path rp::ResourcePipelineManager::getResourceBuildCachePath(const GUID &guid) const
    {
        return change_extension( 
            m_config.buildDirectory / to_string( guid ),
            kCacheFileExtension 
        );
    }

    ///////////////////////////////////////////////////////////////////////////

    rp::TypePair rp::ResourcePipelineManager::detectResourceHandlers(const fs::path &path) const
    {
        auto &importerTypes = getImporterTypes( );
        auto extension = path.extension( ).string( );

        if (!extension.empty( ) && extension[ 0 ] == '.')
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

    ///////////////////////////////////////////////////////////////////////////

    rp::ResourceMetaData rp::ResourcePipelineManager::createDefaultResourceMeta(
        const fs::path &sourceFile
    )
    {
        ResourceMetaData meta;

        auto extension = sourceFile.extension( ).string( );

        // remove period from extension
        if (!extension.empty( ) && extension.front( ) == '.')
            extension.erase( extension.begin( ) );

        utils::MakeLowerCase( extension );

        meta::Type importerType;
        meta::Type processorType;

        std::tie( importerType, processorType ) = 
            detectResourceHandlers( sourceFile );

        // importer does not exist, ignore it.
        if (!importerType.IsValid( ))
            return meta;

        UAssertCatchable( importerType.GetDynamicConstructor( ).IsValid( ),
            "Importer '%s' does not have a default dynamic constructor.",
            importerType.GetName( ).c_str( )
        );

        UAssertCatchable( processorType.IsValid( ),
            "Importer '%s' specified an invalid processor.",
            importerType.GetName( ).c_str( )
        );

        UAssertCatchable( processorType.GetDynamicConstructor( ).IsValid( ),
            "Processor '%s' does not have a default dynamic constructor.",
            processorType.GetName( ).c_str( )
        );

        meta.importer = importerType;
        meta.processor = processorType;

        auto &processorMeta = processorType.GetMeta( );

        auto *processorConfig = 
            processorMeta.GetProperty<ResourceProcessorConfig>( );

        // no config assumes no import options
        if (processorConfig)
        {
            auto optionsType = processorConfig->optionsType;

            UAssertCatchable( optionsType.IsValid( ),
                "Processor '%s' specified invalid options type.",
                processorType.GetName( ).c_str( )
            );

            auto defaultOptionsCtor = optionsType.GetConstructor( );

            UAssertCatchable( defaultOptionsCtor.IsValid( ),
                "Processor options type '%s' missing default constructor.",
                optionsType.GetName( ).c_str( )
            );

            // serialize with the default options
            meta.processorOptions = 
                optionsType.SerializeJson( defaultOptionsCtor.Invoke( ) );
        }
        else
        {
            meta.processorOptions = Json::object { };
        }

        return meta;
    }
}