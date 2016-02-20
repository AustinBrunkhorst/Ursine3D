/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BlendStateList.h
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
        enum BLEND_STATES : unsigned
        {
            BLEND_STATE_DEFAULT = 0,
            BLEND_STATE_ADDITIVE,
            BLEND_STATE_NONE,

            BLEND_STATE_COUNT
        };
    }
}
