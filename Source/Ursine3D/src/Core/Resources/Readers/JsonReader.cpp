#include "UrsinePrecompiled.h"

#include "TextureReader.h"
#include "TextureData.h"

namespace ursine
{
    namespace resources
    {
        TextureReader::TextureReader(void) { }

        ResourceData::Handle TextureReader::Read(ResourceReader &input)
        {
            char poopyTexture[12];

            input.ReadBytes( poopyTexture, 12 );

            // TODO:
            return std::make_shared<TextureData>( );
        }
    }
}