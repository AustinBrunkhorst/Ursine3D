/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FontData.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "FontData.h"
#include "FontReader.h"

#include "GfxAPI.h"

#include <DirectXTex.h>

namespace dx = DirectX;

namespace ursine
{
    namespace resources
    {
        FontData::FontData(BinaryData fntData, TexturePageTable &&table)
            : m_fntData( std::move( fntData ) )
            , m_pages( std::move( table ) )
        {
            auto *graphics = GetCoreSystem( graphics::GfxAPI );

            m_fontHandle = graphics->ResourceMgr.CreateBitmapFont( 
                reinterpret_cast<const uint8_t*>( m_fntData.GetData( ) ), 
                m_fntData.GetSize( )
            );

            for (auto &page : m_pages)
            {
                auto &path = page.first;
                auto &data = page.second;

                dx::TexMetadata meta;

                auto result = GetMetadataFromDDSMemory( 
                    data->GetData( ), 
                    data->GetSize( ), 
                    dx::DDS_FLAGS_NONE,
                    meta 
                );

                UAssertCatchable( result == S_OK,
                    "Unable to load font texture page from memory. Is compression set to DXT5?\npage: %s",
                    page.first.string( ).c_str( )
                );

                auto texture = graphics->ResourceMgr.CreateTexture(
                    reinterpret_cast<const uint8_t*>( data->GetData( ) ),
                    data->GetSize( ),
                    static_cast<unsigned>( meta.width ),
                    static_cast<unsigned>( meta.height )
                );

                m_pageTextureHandles.emplace_back( texture );

                graphics->ResourceMgr.RegisterTexture(
                    m_fontHandle,
                    path.string( ),
                    texture
                );
            }
        }

        FontData::FontData(const FontData &rhs)
            : m_fntData( BinaryData::Copy( rhs.m_fntData ) )
            , m_pages( rhs.m_pages ) { }

        FontData::~FontData(void)
        {
            auto *graphics = GetCoreSystem( graphics::GfxAPI );

            for (auto &handle : m_pageTextureHandles)
                graphics->ResourceMgr.DestroyTexture( handle );

            graphics->ResourceMgr.DestroyBitmapFont( m_fontHandle );
        }

        graphics::GfxHND FontData::GetFontHandle(void) const
        {
            return m_fontHandle;
        }

        void FontData::Write(pipeline::ResourceWriter &output)
        {
            output.Write( m_fntData );

            output.Write( static_cast<unsigned>( m_pages.size( ) ) );

            for (auto &page : m_pages)
            {
                output.Write( page.first.string( ) );
                output.Write( *page.second );
            }
        }

        meta::Type FontData::GetReaderType(void)
        {
            return typeof( FontReader );
        }
    }
}
