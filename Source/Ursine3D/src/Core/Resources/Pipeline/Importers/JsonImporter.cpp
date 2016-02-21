#include "UrsinePrecompiled.h"

#include "JsonImporter.h"

namespace ursine
{
    rp::JsonImporter::JsonImporter(void) { }

    resources::ResourceData::Handle rp::JsonImporter::Import(const fs::path &fileName, const ResourceImportContext &context)
    {
        return nullptr;
    }
}