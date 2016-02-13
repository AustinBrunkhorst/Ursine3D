#pragma once

#include "ResourceImportContext.h"
#include "ResourceConfig.h"
#include "FileSystem.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourceImporter : public meta::Object
            {
            public:
                virtual ResourceDataHandle Import(const fs::path &filename, const ResourceImportContext &context) = 0;
            };
        }
    }
}