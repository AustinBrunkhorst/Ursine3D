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

                ResourceItem::Set generatedResources;

                ResourceBuildStepContext(ResourcePipelineManager *pipeline, ResourceItem::Handle resource);

                ResourceItem::Handle AllocateGeneratedResource(const fs::path &sourceFile);
            };
        }
    }
}