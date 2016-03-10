#include "UrsinePrecompiled.h"

#include "TextureData.h"
#include "TextureReader.h"
#include <Core/CoreSystem.h>
#include "GfxAPI.h"

namespace ursine
{
    namespace resources
    {
        TextureData::TextureData(BinaryData data, unsigned width, unsigned height)
        {
            m_textureHandle = GetCoreSystem( graphics::GfxAPI )->ResourceMgr.CreateTexture(
                reinterpret_cast<const uint8_t*>( data.GetData( ) ), 
                data.GetSize( ), 
                width, 
                height
            );
        }

        TextureData::~TextureData(void)
        {
            GetCoreSystem( graphics::GfxAPI )->ResourceMgr.DestroyTexture( m_textureHandle );
        }

        graphics::GfxHND TextureData::GetTextureHandle(void) const
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

            output.Write( size );

            output.WriteBytes( 
                reinterpret_cast<char*>( data ),
                size
            );
        }

        meta::Type TextureData::GetReaderType(void)
        {
            return typeof( TextureReader );
        }
    }
}
