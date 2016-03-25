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

                ResourceItem::List generatedResources;

                ResourceBuildStepContext(ResourcePipelineManager *pipeline, ResourceItem::Handle resource);

                ResourceItem::Handle AllocateGeneratedResource(const fs::path &sourceFile);
            };
        }
    }
}