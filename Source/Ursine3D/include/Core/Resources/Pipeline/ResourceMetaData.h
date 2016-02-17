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
                Json processorOptions;
            };
        }
    }
}