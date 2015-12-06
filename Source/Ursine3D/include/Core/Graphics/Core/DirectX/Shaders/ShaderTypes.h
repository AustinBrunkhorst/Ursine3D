/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ShaderTypes.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      ShaderTypes.h
Module:         Graphics
Purpose:        defining the types of shaders
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include "ShaderList.h"

namespace ursine
{
    namespace graphics
    {
        enum SHADERTYPE
        {
            SHADERTYPE_VERTEX = 0,
            SHADERTYPE_PIXEL,
            SHADERTYPE_HULL,
            SHADERTYPE_DOMAIN,
            SHADERTYPE_GEOMETRY,
            SHADERTYPE_COMPUTE,

            SHADERTYPE_COUNT
        };
    }
}