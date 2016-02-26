/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DepthStencilStateList.h
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
        enum DEPTH_STATES : unsigned
        {
            DEPTH_STATE_DEPTH_NOSTENCIL = 0,
            DEPTH_STATE_NODEPTH_NOSTENCIL,

            DEPTH_STATE_DEPTH_STENCIL,
            DEPTH_STATE_NODEPTH_STENCIL,

            DEPTH_STATE_POINTLIGHT,
            DEPTH_STATE_PASSDEPTH_WRITESTENCIL,

            DEPTH_STATE_PASSDEPTH_NOSTENCIL,

            DEPTH_STATE_CHECKDEPTH_NOWRITE_NOSTENCIL,

            DEPTH_STATE_COUNT
        };
    }
}
