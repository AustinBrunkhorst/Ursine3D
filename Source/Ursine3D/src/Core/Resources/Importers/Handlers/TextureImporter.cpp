#include "UrsinePrecompiled.h"

#include "TextureImporter.h"

namespace ursine
{
    RESOURCE_HANDLER_DEFINITION( TextureImporter );

    ResourceData *TextureImporter::Import(ResourceImportContext &context)
    {
        return nullptr;
    }

    void TextureImporter::WriteData(ResourceData *data, ResourceOutputContext &context)
    {
        
    }

    ResourceData *TextureImporter::LoadData(ResourceOutputContext &context)
    {
        return nullptr;
    }
}