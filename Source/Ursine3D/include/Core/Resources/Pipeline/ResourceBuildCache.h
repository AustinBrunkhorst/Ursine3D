#pragma once

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourceItem;

            struct ResourceBuildCache
            {
                // Determines if this asset built a preview
                bool hasPreview;

                // Runtime type of the processed resource
                meta::Type processedType;

                // Resources that were generated from this resource
                std::unordered_set<std::shared_ptr<ResourceItem>> generatedResources;
            };
        }
    }
}