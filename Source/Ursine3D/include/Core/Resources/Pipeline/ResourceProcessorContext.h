#pragma once

#include "ResourceBuildStepContext.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourcePipelineManager;

            struct ResourceProcessorContext : ResourceBuildStepContext
            {
                ResourceProcessorContext(ResourcePipelineManager *pipeline, ResourceItem::Handle resource)
                    : ResourceBuildStepContext( pipeline, resource ) { }
            };
        }
    }
}