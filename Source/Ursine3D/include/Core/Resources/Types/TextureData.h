/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TextureData.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
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
        class TextureData : public ResourceData
        {
            RESOURCE_DATA(TextureData);

        public:
            TextureData(BinaryData data, unsigned width, unsigned height);
            ~TextureData(void);

            graphics::GfxHND GetTextureHandle(void) const;

        private:
            graphics::GfxHND m_textureHandle;

            void Write(pipeline::ResourceWriter &output) override;

            meta::Type GetReaderType(void) override;
        } Meta(Register);
    }
}