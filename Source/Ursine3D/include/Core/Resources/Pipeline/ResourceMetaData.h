#pragma once

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            struct ResourceMetaData
            {
                meta::Type importer;
                meta::Type processor;
                Json processorOptions;
            };
        }
    }
}