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
        enum SHADER_TYPES
        {
            SHADER_BASIC = 0,
            SHADER_QUAD,
            SHADER_DIFFUSE,
            SHADER_NORMAL,
            SHADER_DEFFERED_TEXTURE,
            SHADER_BILLBOARD2D,
            
            SHADER_DEFERRED_DEPTH,
            SHADER_DEFERRED_DEPTH_NORM,
            
            SHADER_UI,

            //lights need to go after this, post processing stuffz...
            SHADER_SPOT_LIGHT,
            SHADER_POINT_LIGHT, //this order is important, DO NOT SWAP
            SHADER_DIRECTIONAL_LIGHT,

            /////////////////////////////////////////////////////////
            // and past this is debug!
            SHADER_PRIMITIVE,

            

            //shaders not part of the regular pipeline
            SHADER_POINT,
            SHADER_SHADOW,

            SHADER_MOUSEPOSITION,

            SHADER_PARTICLE,

            SHADER_COUNT
        };
    }
}