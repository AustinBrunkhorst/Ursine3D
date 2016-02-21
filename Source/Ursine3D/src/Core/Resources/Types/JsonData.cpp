#include "UrsinePrecompiled.h"

#include "TextureData.h"
#include "TextureReader.h"

namespace ursine
{
    namespace resources
    {
        void TextureData::Write(pipeline::ResourceWriter &output)
        {
            output << "PoopyTexture";
        }

        meta::Type TextureData::GetReaderType(void)
        {
            return typeof( TextureReader );
        }
    }
}