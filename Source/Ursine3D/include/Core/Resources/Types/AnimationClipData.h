/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationClipData.h
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
#include "AnimationInfo.h"

namespace ursine
{
    namespace resources
    {
        class AnimationClipData : public ResourceData
        {
            RESOURCE_DATA;

        public:
            AnimationClipData(const graphics::ufmt_loader::AnimInfo &animInfo);
            ~AnimationClipData(void);

            graphics::GfxHND GetAnimeHandle(void) const;

            void Write(pipeline::ResourceWriter &output) override;

        private:
            graphics::GfxHND m_animHandle;

            meta::Type GetReaderType(void) override;
        } Meta( Register );
    }
}