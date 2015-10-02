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
        enum SHADERDEF
        {
            VERTEX_SHADER = 0,
            PIXEL_SHADER,
            HULL_SHADER,
            DOMAIN_SHADER,
            GEOMETRY_SHADER,
            DEF_COUNT
        };
    }
}