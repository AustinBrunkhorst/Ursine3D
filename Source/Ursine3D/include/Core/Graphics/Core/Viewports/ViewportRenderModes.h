/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      ViewportRenderModes.h
Module:         Graphics
Purpose:        Render modes for viewports
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

enum ViewportRenderMode
{
    VIEWPORT_RENDER_FORWARD = 0, //forward rendering, camera-based lighting, super fast! NOTE: Lights will not work
    VIEWPORT_RENDER_DEFERRED, //deferred rendering, cool point lights! Expensive

    RENDER_COUNT
};
