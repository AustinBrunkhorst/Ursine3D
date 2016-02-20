/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RenderableTypes.h
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
        enum RENDERABLE_TYPE : unsigned
        {
            RENDERABLE_MODEL3D = 0,     // 3d model
            RENDERABLE_BILLBOARD2D,     // 2D billboard
            RENDERABLE_SPRITE_TEXT,      // sprite text
            RENDERABLE_LIGHT,           // light
            RENDERABLE_PS,              // particle system
            RENDERABLE_TYPE_COUNT,
            RENDERABLE_OVERDRAW
        };
    }
}