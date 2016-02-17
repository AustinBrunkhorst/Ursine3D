#include "UrsinePrecompiled.h"

#include "DefaultImporter.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            ResourceData::Handle ResourceImporter::Import(const fs::path &filename, const ResourceImportContext &context)
            {
                // do nothing
                return nullptr;
            }
        }
    }
}