#pragma once

#include "ResourceWriter.h"
#include "ResourceConfig.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourceTypeWriter : public meta::Object
            {
            public:
                virtual void Write(const ResourceWriter &output, ResourceDataHandle resource) = 0;
            };
        }
    }
}