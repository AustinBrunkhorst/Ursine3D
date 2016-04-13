/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FontManager.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "FontManager.h"

namespace ursine
{
    namespace graphics
    {
        void FontManager::Initialize(void)
        {
            m_loadingFont = false;
        }

        void FontManager::Uninitialize(void)
        {
        }

        GfxHND FontManager::CreateBitmapFont(const uint8_t *binaryData, size_t binarySize)
        {
            GfxHND handle;
            auto *hnd = HND_RSRCE(handle);

            unsigned internalID = static_cast<unsigned>(m_fontCache.size( ));

            // emplace into map
            auto &font = m_fontCache.emplace(internalID, BitmapFont( )).first->second;

            // load data
            font.Load( binaryData, binarySize );

            // initialize handle
            hnd->ID_ = SANITY_RESOURCE;
            hnd->Type_ = ID_FONT;
            hnd->Index_ = internalID;

            return handle;
        }

        void FontManager::DestroyBitmapFont(GfxHND &fontHandle)
        {
            auto *hnd = HND_RSRCE(fontHandle);

            UAssert(hnd->ID_ == SANITY_RESOURCE,
                "Attempted to get font with invalid handle!"
            );
            
            UAssert(hnd->Type_ == ID_FONT,
                "Attempted to get font with handle of invalid type!"
            );

            auto search = m_fontCache.find( hnd->Index_ );

            UAssert( search != m_fontCache.end( ),
                "Failed to destroy font.\nID: %i",
                hnd->Index_
            );

            m_fontCache.erase( search );

            auto texSearch = m_textureCache.find( hnd->Index_ );

            if(texSearch != m_textureCache.end( ))
            {
                m_textureCache.erase( texSearch );
            }

            fontHandle = 0;
        }

        void FontManager::RegisterTexture(GfxHND fontHandle, const std::string &fontName, GfxHND textureHandle)
        {
             auto *hnd = HND_RSRCE(fontHandle);

            UAssert(hnd->ID_ == SANITY_RESOURCE,
                "Attempted to get font with invalid handle!"
            );
            
            UAssert(hnd->Type_ == ID_FONT,
                "Attempted to get font with handle of invalid type!"
            );

            auto search = m_fontCache.find( hnd->Index_ );

            UAssert( search != m_fontCache.end( ),
                "Failed to get font.\nID: %i",
                hnd->Index_
            );

            m_textureCache[ hnd->Index_ ].emplace( fontName, textureHandle );
        }

        const BitmapFont &FontManager::GetBitmapFont(GfxHND index)
        {
            auto search = m_fontCache.find( index & 0xFFFF );

            UAssert( search != m_fontCache.end( ),
                "Failed to get font.\nID: %i",
                index
            );

            return search->second;
        }

        GfxHND FontManager::GetTextureHandle(GfxHND fontHandle, const std::string &textureName)
        {
            auto *hnd = HND_RSRCE(fontHandle);

            UAssert(hnd->ID_ == SANITY_RESOURCE,
                "Attempted to get font with invalid handle!"
            );
            
            UAssert(hnd->Type_ == ID_FONT,
                "Attempted to get font with handle of invalid type!"
            );

            auto search = m_fontCache.find( hnd->Index_ );

            UAssert( search != m_fontCache.end( ),
                "Failed to get font.\nID: %i",
                hnd->Index_
            );

            auto handle = m_textureCache[ hnd->Index_ ].find( textureName );

            if(!(handle != m_textureCache[ hnd->Index_ ].end()))
            {
                std::cout << m_textureCache.size( ) << std::endl;
            }

            UAssert( handle != m_textureCache[ hnd->Index_ ].end( ),
                "Failed to find font texture.\nID: %i\nFont: %s",
                hnd->Index_,
                textureName.c_str( )
            );

            return handle->second;
        }

        void FontManager::LockFontManager(void)
        {
            m_loadingFont = true;
        }

        void FontManager::UnlockFontManager(void)
        {
            m_loadingFont = false;
        }

        bool FontManager::IsLoading(void) const
        {
            return m_loadingFont;
        }
    }
}