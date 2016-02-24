#pragma once

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            struct ResourceMetaData
            {
                // ResourceImporter type that imports this resource
                meta::Type importer;

                // ResourceProcessor type that processes this resource
                meta::Type processor;

                // Options used by the processor
                Json processorOptions;
            };
        }
    }
}