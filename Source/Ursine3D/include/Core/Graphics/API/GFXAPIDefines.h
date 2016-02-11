/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GFXAPIDefines.h
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
File Name:      GFXAPIDefines.h
Module:         Graphics
Purpose:        global API definitions
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once
#include "BlendStateList.h"
#include "ShaderList.h"
#include "RenderableTypes.h"

namespace ursine
{
    namespace graphics
    {
        typedef unsigned long long GfxHND;      // universal graphics handle
        typedef unsigned long long RenderMask;  // mask to cull rendering on a layer level
    }
}

