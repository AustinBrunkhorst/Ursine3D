/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ShaderList.h
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
        enum SHADER_LIST : unsigned
        {
            SHADER_BASIC = 0,
            SHADER_QUAD,

            SHADER_FRAGMENTATION,
            
            SHADER_DEFERRED_DEPTH,
            SHADER_DEFERRED_DEPTH_STATIC,
            SHADER_DEFERRED_DEPTH_NORM,

            SHADER_OUTLINE,
            
            SHADER_UI,

            //lights need to go after this, post processing stuffz...
            SHADER_SPOT_LIGHT,
            SHADER_SPOT_LIGHT_NO_SHADOW,
            SHADER_POINT_LIGHT, //this order is important, DO NOT SWAP
            SHADER_DIRECTIONAL_LIGHT,
            SHADER_EMISSIVE,

            // needed for overdraw to work
            SHADER_OVERDRAW_MODEL,

            // forward pass on particles and billboards
            SHADER_PARTICLE,
            SHADER_VELOCITY_PARTICLE,
            SHADER_BILLBOARD2D,
            SHADER_SPRITE_TEXT,

            /////////////////////////////////////////////////////////
            // and past this is debug!
            SHADER_PRIMITIVE,
            SHADER_MOUSEPOSITION,

            //shaders not part of the regular pipeline
            SHADER_FORWARD,
            SHADER_POINT,
            SHADER_SHADOW_PASS,

            SHADER_COUNT,

            SHADER_OVERRIDE
        };
    }
}