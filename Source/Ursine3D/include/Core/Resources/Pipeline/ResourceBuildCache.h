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

                // Resource that generated this resource.
                // NULL GUID if it doesn't exist
                GUID parent;

                // Resources that were generated from this resource
                std::unordered_set<GUID, GUIDHasher> generatedResources;
            };
        }
    }
}