#pragma once

#include "ResourceItem.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourcePipelineManager;

            struct ResourceBuildStepContext
            {
                ResourcePipelineManager *pipeline;
                ResourceItem::Handle resource;

                ResourceBuildStepContext(ResourcePipelineManager *pipeline, ResourceItem::Handle resource)
                    : pipeline( pipeline )
                    , resource( resource ) { }
            };
        }
    }
}