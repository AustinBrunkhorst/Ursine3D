#pragma once

#include "ResourcePipelineConfig.h"
#include "ResourcePipelineEvent.h"

#include "ResourceDirectoryNode.h"
#include "ResourceItem.h"

#include "ResourceBuildContext.h"

#include "BuiltInResourceConfig.h"

#include "GUID.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourcePipelineManager
                : public EventDispatcher<ResourcePipelineEventType>
                , public fs::FileWatchListener
            {
            public:
                ResourcePipelineManager(void);
                ~ResourcePipelineManager(void);

                const ResourcePipelineConfig &GetConfig(void) const;
                void SetConfig(const ResourcePipelineConfig &config);

                ResourceDirectoryNode *GetRootResourceDirectory(void);

                void Build(void);

                void WatchResourceDirectory(void);
                void StopWatchingResourceDirectory(void);
                bool IsWatchingResourceDirectory(void) const;

                void InvalidateResourceMeta(ResourceItem::Handle resource);
                void InvalidateResourceCache(ResourceItem::Handle resource);

                ResourceItem::Handle GetItem(const GUID &guid);
                ResourceItem::List GetItemsByType(const meta::Type &type);
            private:
                ResourcePipelineConfig m_config;

                std::unordered_map<GUID, ResourceItem::Handle, GUIDHasher> m_database;
                std::unordered_map<fs::path, ResourceDirectoryNode *, fs::PathHasher> m_pathToDirectoryNode;

                ResourceDirectoryNode *m_rootDirectory;

                std::thread m_buildWorkerThread;

                fs::FileWatcher m_fileWatcher;
                fs::WatchID m_resourceDirectoryWatch;

                ResourcePipelineManager(const ResourcePipelineManager &rhs) = delete;
                ResourcePipelineManager &operator=(const ResourcePipelineManager &rhs) = delete;

                ///////////////////////////////////////////////////////////////
                // Registration
                ///////////////////////////////////////////////////////////////

                // scans the resource directory and registers all assets
                void registerResources(
                    ResourceDirectoryNode *directory,
                    const fs::path &directoryName
                );

                // determines how to handle a resource
                ResourceItem::Handle registerResource(const fs::path &fileName, bool isGenerated = false);

                // imports an existing resource (with existing meta file)
                ResourceItem::Handle addExistingResource(
                    const fs::path &fileName, 
                    const fs::path &metaFileName,
                    bool isGenerated = false
                );

                // imports a resource with default options and writes the meta file
                ResourceItem::Handle addDefaultResource(
                    const fs::path &fileName, 
                    const fs::path &metaFileName
                );

                // creates a resource in the database
                ResourceItem::Handle allocateResource(
                    const fs::path &fileName,
                    const fs::path &metaFileName,
                    const GUID &guid = GUIDGenerator( )( )
                );

                // inserts this resource into the directory tree
                void insertResource(ResourceItem::Handle resource);

                // determines if the given directory should be treated as a resource
                bool isDirectoryResource(const fs::path &directory);

                // loads the build cache for this resource
                void loadResourceBuildCache(ResourceItem::Handle resource);

                ///////////////////////////////////////////////////////////////
                // Building
                ///////////////////////////////////////////////////////////////

                void buildResources(void);

                void buildResource(ResourceBuildContext &context);

                void addGeneratedResources(
                    ResourceBuildContext &context, 
                    std::list<ResourceBuildContext> &outGenerated
                );

                void buildResourcePreview(ResourceBuildContext &context);

                bool buildIsInvalidated(ResourceItem::Handle resource);

                ///////////////////////////////////////////////////////////////
                // File Watching
                ///////////////////////////////////////////////////////////////
                
                void handleFileAction(
                    fs::WatchID watchid,
                    const std::string &dir,
                    const std::string &filename,
                    fs::Action action,
                    std::string oldFilename = ""
                ) override;

                ///////////////////////////////////////////////////////////////
                // Utilities
                ///////////////////////////////////////////////////////////////

                // gets build file name for this resource
                fs::path getResourceBuildFile(const GUID &guid) const;
                fs::path getResourceBuildPreviewFile(const GUID &guid) const;
                fs::path getResourceBuildCacheFile(const GUID &guid) const;

                TypePair detectResourceHandlers(const fs::path &path) const;
            };
        }
    }
}