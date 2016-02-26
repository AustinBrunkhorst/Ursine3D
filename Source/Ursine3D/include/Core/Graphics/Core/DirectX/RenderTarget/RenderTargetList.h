/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RenderTargetList.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    namespace graphics
    {
        enum RENDER_TARGETS : unsigned
        {
            //reserved targets
            RENDER_TARGET_SWAPCHAIN = 0,
            RENDER_TARGET_LIGHTMAP,
            RENDER_TARGET_DEFERRED_NORMAL,
            RENDER_TARGET_DEFERRED_COLOR,
            RENDER_TARGET_DEFERRED_SPECPOW,
            RENDER_TARGET_DEBUG,

            //ui
            RENDER_TARGET_UI,
            RENDER_TARGET_UI2,
            RENDER_TARGET_UI3,
            RENDER_TARGET_UI4,

            //viewport targets
            RENDER_TARGET_VIEWPORT_1,
            RENDER_TARGET_VIEWPORT_2,
            RENDER_TARGET_VIEWPORT_3,
            RENDER_TARGET_VIEWPORT_4,
            RENDER_TARGET_VIEWPORT_5,

            RENDER_TARGET_COUNT,

            RENDER_TARGET_MAX_VIEWPORT = RENDER_TARGET_COUNT - RENDER_TARGET_VIEWPORT_1
        };
    }
}
