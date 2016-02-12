#pragma once

#include "ResourceImportContext.h"
#include "ResourceConfig.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourceImporter : public meta::Object
            {
            public:
                virtual ResourceDataHandle Import(const std::string &filename, const ResourceImportContext &context) = 0;
            };
        }
    }
}