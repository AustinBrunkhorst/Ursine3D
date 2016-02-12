#pragma once

#include "ResourceProcessorContext.h"
#include "ResourceConfig.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline 
        {
            class ResourceProcessor : public meta::Object
            {
            public:
                virtual ResourceDataHandle Process(ResourceDataHandle input, const ResourceProcessorContext &context) = 0;
            };
        }
    }
}