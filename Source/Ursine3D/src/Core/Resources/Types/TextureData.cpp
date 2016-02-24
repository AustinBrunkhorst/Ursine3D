#include "UrsinePrecompiled.h"

#include "TextureData.h"
#include "TextureReader.h"
#include <Core/CoreSystem.h>
#include "GfxAPI.h"

namespace ursine
{
    namespace resources
    {
        TextureData::TextureData(void *bytes, size_t size, unsigned width, unsigned height)
        {
            m_textureHandle = GetCoreSystem( graphics::GfxAPI )->ResourceMgr.CreateTexture(
                reinterpret_cast<uint8_t*>(bytes), 
                size, 
                width, 
                height
            );
        }

        TextureData::~TextureData(void)
        {
            GetCoreSystem( graphics::GfxAPI )->ResourceMgr.DestroyTexture( m_textureHandle );
        }

        graphics::GfxHND TextureData::GetTextureHandle() const
        {
            return m_textureHandle;
        }

        void TextureData::Write(pipeline::ResourceWriter &output)
        {
            uint8_t *data;
            size_t size;

            GetCoreSystem( graphics::GfxAPI )->ResourceMgr.GetBinaryInformation(
                m_textureHandle, 
                &data, 
                size
            );

            output << size;

            output.WriteBytes( 
                reinterpret_cast<char*>(data),
                size
            );
        }

        meta::Type TextureData::GetReaderType(void)
        {
            return typeof( TextureReader );
        }
    }
}
