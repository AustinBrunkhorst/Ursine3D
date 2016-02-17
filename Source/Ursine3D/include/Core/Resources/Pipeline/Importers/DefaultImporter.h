#pragma once

#include "ResourceImporter.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class DefaultImporter : public ResourceImporter
            {
            public:
                ResourceData::Handle Import(const fs::path &filename, const ResourceImportContext &context) override;
            } Meta(Enable);
        }
    }
}