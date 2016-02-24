#pragma once

#include "ResourceBuildStepContext.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourcePipelineManager;

            struct ResourceImportContext : ResourceBuildStepContext
            {
                ResourceImportContext(ResourcePipelineManager *pipeline, ResourceItem::Handle resource)
                    : ResourceBuildStepContext( pipeline, resource ) { }
            };
        }
    }
}