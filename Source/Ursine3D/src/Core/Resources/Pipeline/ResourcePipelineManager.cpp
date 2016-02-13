#include "UrsinePrecompiled.h"

#include "ResourcePipelineManager.h"

namespace ursine 
{
    namespace resources
    {
        namespace pipeline
        {
            void ResourcePipelineManager::SetConfig(const ResourcePipelineConfig &config)
            {
                m_config = config;
            }
        }
    }
}