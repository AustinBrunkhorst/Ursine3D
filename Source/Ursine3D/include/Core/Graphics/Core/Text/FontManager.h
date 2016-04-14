/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FontManager.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <atomic>

#include "GfxDefines.h"
#include "BitmapFont.h"

namespace ursine
{
    namespace graphics
    {
        class FontManager
        {
            friend class GfxManager;
        public:
            void Initialize(void);
            void Uninitialize(void);

            // create a font, returns the handle to it
            GfxHND CreateBitmapFont(const uint8_t *binaryData, size_t binarySize);

            void DestroyBitmapFont(GfxHND &fontHandle);

            // registers the texture handle of a font
            void RegisterTexture(GfxHND fontHandle, const std::string &fontName, GfxHND textureHandle);

            // gets a font
            const BitmapFont &GetBitmapFont(GfxHND index);

            // gets a texture handle for a font
            GfxHND GetTextureHandle(GfxHND fontHandle, const std::string &textureName);

            // locking/unlocking for loading
            void LockFontManager(void);
            void UnlockFontManager(void);

            bool IsLoading(void) const;

        private:
            // actual font data
            std::unordered_map<unsigned, BitmapFont> m_fontCache;

            // lookup for the texture handles for specific fonts
            std::unordered_map<unsigned, std::unordered_map<std::string, GfxHND>> m_textureCache;

            std::atomic<bool> m_loadingFont;
        };
    }
}