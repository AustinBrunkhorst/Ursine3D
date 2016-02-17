#pragma once

#include "ResourceData.h"
#include "ResourceImportContext.h"
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
                virtual ResourceData::Handle Import(const fs::path &filename, const ResourceImportContext &context);
            };
        }
    }
}