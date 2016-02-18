#pragma once

#include "ResourcePipelineConfig.h"
#include "ResourceItem.h"
#include "GUID.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourcePipelineManager
            {
            public:
                void SetConfig(const ResourcePipelineConfig &config);

                void Build(void);

                void InvalidateResourceMeta(ResourceItem::Handle resource);
            private:
                ResourcePipelineConfig m_config;

                std::unordered_map<GUID, ResourceItem::Handle, GUIDHasher> m_database;

                std::thread m_buildWorkerThread;

                ///////////////////////////////////////////////////////////////
                // Registration
                ///////////////////////////////////////////////////////////////

                // scans the resource directory and registers all assets
                void registerResources(void);
                
                // determines how to handle a resource
                void registerResource(const fs::path &fileName);

                // imports an existing resource (with existing meta file)
                void addExistingResource(
                    const fs::path &fileName, 
                    const fs::path &metaFileName
                );

                // imports a resource with default options and writes the meta file
                void addDefaultResource(
                    const fs::path &fileName, 
                    const fs::path &metaFileName
                );

                // creates a resource in the database
                ResourceItem::Handle allocateResource(
                    const fs::path &fileName,
                    const fs::path &metaFileName,
                    const GUID &guid = GUIDGenerator( )( )
                );

                ///////////////////////////////////////////////////////////////
                // Building
                ///////////////////////////////////////////////////////////////

                void buildResources(void);
                void buildResource(ResourceItem::Handle resource);

                ///////////////////////////////////////////////////////////////
                // Utilities
                ///////////////////////////////////////////////////////////////

                // gets build file name for this resource
                fs::path getResourceBuildFile(const GUID &guid) const;
            };
        }
    }
}