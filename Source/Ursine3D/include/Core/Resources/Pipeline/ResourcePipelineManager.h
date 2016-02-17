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

            private:
                ResourcePipelineConfig m_config;

                std::unordered_map<GUID, ResourceItem::Handle, GUIDHasher> m_database;

                // scans the resource directory and registers all assets
                void registerResources(void);
                
                // determines how to handle a resource
                void registerResource(const fs::path &filename);

                void initializeResource(const fs::path &filename, const Json &metaData);

                Json createDefaultResourceMetaData(const fs::path &filename);
            };
        }
    }
}