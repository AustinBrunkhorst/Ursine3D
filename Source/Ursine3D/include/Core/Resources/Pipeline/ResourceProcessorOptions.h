#pragma once

#include "Meta.h"

#define RESOURCE_PROCESSOR_OPTIONS META_OBJECT

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourceProcessorOptions : public meta::Object
            {
            public:
                typedef std::shared_ptr<ResourceProcessorOptions> Handle;
            };
        }
    }
}