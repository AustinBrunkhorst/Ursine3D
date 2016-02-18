#include "UrsinePrecompiled.h"

#include "TextureImporter.h"
#include "TextureData.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            TextureImporter::TextureImporter(void) { }

            ResourceData::Handle TextureImporter::Import(const fs::path &fileName, const ResourceImportContext &context)
            {
                return std::make_shared<TextureData>( );
            }
        }
    }
}