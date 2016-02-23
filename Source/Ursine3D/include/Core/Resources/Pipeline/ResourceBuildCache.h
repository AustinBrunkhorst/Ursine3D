#pragma once

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            struct ResourceBuildCache
            {
                // Runtime type of the processed resource
                meta::Type processedType;

                // Resources that were generated from this resource
                std::vector<GUID> generatedResources;
            };
        }
    }
}