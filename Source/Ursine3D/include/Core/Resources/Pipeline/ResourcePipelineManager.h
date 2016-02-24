#pragma once

#include "ResourcePipelineConfig.h"
#include "ResourcePipelineEvent.h"

#include "ResourceDirectoryNode.h"
#include "ResourceItem.h"

#include "ResourceBuildContext.h"

#include "GUID.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourcePipelineManager
                : public EventDispatcher<ResourcePipelineEventType>
            {
            public:
                ResourcePipelineManager(void);

                const ResourcePipelineConfig &GetConfig(void) const;
                void SetConfig(const ResourcePipelineConfig &config);

                ResourceDirectoryNode *GetRootResourceDirectory(void);

                void Build(void);

                void InvalidateResourceMeta(ResourceItem::Handle resource);
                void InvalidateResourceCache(ResourceItem::Handle resource);

                ResourceItem::Handle GetItem(const GUID &guid);
            private:
                ResourcePipelineConfig m_config;

                std::unordered_map<GUID, ResourceItem::Handle, GUIDHasher> m_database;

                ResourceDirectoryNode *m_rootDirectory;

                std::thread m_buildWorkerThread;

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
                ResourceItem::Handle registerResource(const fs::path &fileName);

                // imports an existing resource (with existing meta file)
                ResourceItem::Handle addExistingResource(
                    const fs::path &fileName, 
                    const fs::path &metaFileName
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

                // loads the build cache for this resource
                void loadResourceBuildCache(ResourceItem::Handle resource);

                ///////////////////////////////////////////////////////////////
                // Building
                ///////////////////////////////////////////////////////////////

                void buildResources(void);

                void buildResource(ResourceBuildContext &context);
                void buildResourcePreview(ResourceBuildContext &context);

                bool buildIsInvalidated(ResourceItem::Handle resource);

                ///////////////////////////////////////////////////////////////
                // Utilities
                ///////////////////////////////////////////////////////////////

                // gets build file name for this resource
                fs::path getResourceBuildFile(const GUID &guid) const;
                fs::path getResourceBuildPreviewFile(const GUID &guid) const;
                fs::path getResourceBuildCacheFile(const GUID &guid) const;
            };
        }
    }
}