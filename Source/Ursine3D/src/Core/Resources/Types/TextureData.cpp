#include "UrsinePrecompiled.h"

#include "TextureData.h"
#include "TextureReader.h"

namespace ursine
{
    namespace resources
    {
        TextureData::TextureData(void *bytes, size_t size, unsigned width, unsigned height)
            : m_bytes( new uint8[ size ] )
            , m_size( size )
            , m_width( width )
            , m_height( height )
        {
            memcpy( m_bytes, bytes, size );
        }

        TextureData::~TextureData(void)
        {
            delete[] m_bytes;
        }

        uint8 *TextureData::GetBytes(void) const
        {
            return m_bytes;
        }

        size_t TextureData::GetSize(void) const
        {
            return m_size;
        }

        unsigned TextureData::GetWidth(void) const
        {
            return m_width;
        }

        unsigned TextureData::GetHeight(void) const
        {
            return m_height;
        }

        void TextureData::Write(pipeline::ResourceWriter &output)
        {
            output << m_size;

            output.WriteBytes( 
                reinterpret_cast<char*>( m_bytes ), 
                m_size 
            );
        }

        meta::Type TextureData::GetReaderType(void)
        {
            return typeof( TextureReader );
        }
    }
}