#include "UrsinePrecompiled.h"

#include "TextureImporter.h"
#include "TextureData.h"

namespace ursine
{
    rp::TextureImporter::TextureImporter(void) { }

    resources::ResourceData::Handle rp::TextureImporter::Import(const fs::path &fileName, const ResourceImportContext &context)
    {
        return std::make_shared<TextureData>( );
    }
}