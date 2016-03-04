/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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
            RESOURCE_DATA;

        public:
            FontData(uint8_t *binaryData, size_t binarySize);
            ~FontData(void);

            graphics::GfxHND GetFontHandle(void) const;

        private:
            void Write(pipeline::ResourceWriter &output) override;
            meta::Type GetReaderType(void) override;

            graphics::GfxHND m_fontHandle;
            uint8_t *m_binaryData;
            size_t m_binarySize;

        } Meta(Register);
    }
}