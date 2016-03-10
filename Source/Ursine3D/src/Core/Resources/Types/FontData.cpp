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

namespace ursine
{
    namespace resources
    {
        FontData::FontData(BinaryData fntData, TexturePageTable &&table)
            : m_fntData( std::move( fntData ) )
            , m_pages( std::move( table ) )
        {
            /*auto *graphics = GetCoreSystem( graphics::GfxAPI );

            m_fontHandle = graphics->ResourceMgr.CreateBitmapFont( nullptr, 0 );

            graphics->ResourceMgr.RegisterTexture(
                m_fontHandle,
                "NameOfTheFont_0.DDS",
                static_cast<GfxHND>( 0 )
            );*/
        }

        FontData::FontData(const FontData &rhs)
            : m_fntData( BinaryData::Copy( rhs.m_fntData ) )
            , m_pages( rhs.m_pages ) { }

        FontData::~FontData(void) { }

        graphics::GfxHND FontData::GetFontHandle(void) const
        {
            return m_fontHandle;
        }

        void FontData::Write(pipeline::ResourceWriter &output)
        {
            output.Write( m_fntData );

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
