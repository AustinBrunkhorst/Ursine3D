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

            private:
                ResourcePipelineConfig m_config;

                std::unordered_map<GUID, ResourceItem::Handle, GUIDHasher> m_database;
            };
        }
    }
}