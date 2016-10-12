/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FontData.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceData.h"
#include "GfxDefines.h"

namespace ursine
{
    namespace resources
    {
        class FontData : public ResourceData
        {
            RESOURCE_DATA(FontData);

        public:
            typedef std::unordered_map<
                fs::path,
                BinaryData::Handle,
                fs::PathHasher
            > TexturePageTable;

            FontData(BinaryData fntData, TexturePageTable &&table);
            FontData(const FontData &rhs);
            ~FontData(void);

            graphics::GfxHND GetFontHandle(void) const;

        private:
            graphics::GfxHND m_fontHandle;

            std::vector<graphics::GfxHND> m_pageTextureHandles;

            BinaryData m_fntData;
            TexturePageTable m_pages;

            void Write(pipeline::ResourceWriter &output) override;
            meta::Type GetReaderType(void) override;

        } Meta(Register);
    }
}