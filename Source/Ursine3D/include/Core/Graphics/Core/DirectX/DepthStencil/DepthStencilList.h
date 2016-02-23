/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DepthStencilList.h
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
        enum DEPTH_STENCIL_LIST : unsigned
        {
            DEPTH_STENCIL_MAIN = 0,
            DEPTH_STENCIL_SHADOWMAP,
            DEPTH_STENCIL_OVERDRAW,

            DEPTH_STENCIL_COUNT
        };
    }
}
