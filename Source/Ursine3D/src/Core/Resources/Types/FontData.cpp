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
#include "CoreSystem.h"
#include "GfxAPI.h"

namespace ursine
{
    namespace resources
    {
        static char magicValues[] = { 'U', 'F', 'T' };

        FontData::FontData(uint8_t *binaryData, size_t binarySize)
            : m_binarySize( binarySize )
        {
            // read in magic, ensure it's valid

            // read in binary for the .fnt file, send to graphics
            m_fontHandle = GetCoreSystem(graphics::GfxAPI)->ResourceMgr.CreateBitmapFont( nullptr, 0 );

            // load in all textures into textureManager, save each handle

            // register each texture with the font manager
            GetCoreSystem(graphics::GfxAPI)->ResourceMgr.RegisterTexture( 
                m_fontHandle, 
                "NameOfTheFont", 
                static_cast<GfxHND>(0) 
            );

            // make a copy
            m_binaryData = new uint8_t[ binarySize ];
            memcpy( m_binaryData, binaryData, m_binarySize );
        }

        FontData::~FontData(void)
        {
            delete m_binaryData;
            m_binaryData = nullptr;
        }

        graphics::GfxHND FontData::GetFontHandle(void) const
        {
            return m_fontHandle;
        }

        void FontData::Write(pipeline::ResourceWriter &output)
        {
            // write magic number
            output.WriteBytes( magicValues, 3 );
            
            // write the size
            output.Write( m_binarySize );

            // write the actual data
            output.WriteBytes( m_binaryData, m_binarySize );

            // write each texture as:
                // Name
                // binary size
                // binary
        }

        meta::Type FontData::GetReaderType(void)
        {
            return typeof( FontData );
        }
    }
}
