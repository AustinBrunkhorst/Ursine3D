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

/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      RenderableTypes.h
Module:         Graphics
Purpose:        enum of renderable types
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

namespace ursine
{
    namespace graphics
    {
        enum RENDERABLE_TYPE
        {
            RENDERABLE_MODEL3D = 0,     //3d model
            RENDERABLE_BILLBOARD2D,     //2D billboard
            RENDERABLE_LIGHT,           //light
            RENDERABLE_PRIMITIVE,       //wireframe primitives
            
            RENDERABLE_TYPE_COUNT
        };
    }
}