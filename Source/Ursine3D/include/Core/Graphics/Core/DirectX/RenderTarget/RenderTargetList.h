/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      RenderTargetList.h
Module:         Graphics
Purpose:        enum of render targets
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

enum RENDER_TARGETS
{
    //reserved targets
    RENDER_TARGET_SWAPCHAIN = 0,
    RENDER_TARGET_LIGHTMAP,
    RENDER_TARGET_DEFERRED_NORMAL,
    RENDER_TARGET_DEFERRED_COLOR,
    RENDER_TARGET_DEFERRED_SPECPOW,
    RENDER_TARGET_DEBUG,

    //ui
    RENDER_TARGET_UI,
    RENDER_TARGET_UI2,
    RENDER_TARGET_UI3,
    RENDER_TARGET_UI4,

    //viewport targets
    RENDER_TARGET_VIEWPORT_1,
    RENDER_TARGET_VIEWPORT_2,
    RENDER_TARGET_VIEWPORT_3,
    RENDER_TARGET_VIEWPORT_4,
    RENDER_TARGET_VIEWPORT_5,

    RENDER_TARGET_COUNT,

    RENDER_TARGET_MAX_VIEWPORT = RENDER_TARGET_COUNT - RENDER_TARGET_VIEWPORT_1
};
