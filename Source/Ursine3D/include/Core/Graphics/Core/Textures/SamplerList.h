/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SamplerList.h
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
        enum SAMPLER_STATES : unsigned
        {
            SAMPLER_STATE_WRAP_TEX = 0,
            SAMPLER_STATE_NO_WRAP_TEX,
            SAMPLER_STATE_NO_FILTERING,

            SAMPLER_STATE_SHADOW,

            SAMPLER_STATE_COUNT
        };
    }
}