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

enum RENDERABLE_TYPE
{
    RENDERABLE_MODEL3D = 0, //3d model
    RENDERABLE_BILLBOARD2D, //2D billboard
    RENDERABLE_DIRECTION_LIGHT, //directional light, applied to all objects
    RENDERABLE_POINT_LIGHT, //positional light
    RENDERABLE_PRIMITIVE, //wireframe primitives

    RENDERABLE_TYPE_COUNT
};
