/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      GfxConfig.h
Module:         Graphics
Purpose:        config for creating graphics
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include <string>

namespace ursine
{
    struct GfxConfig
    {
        unsigned WindowHeight_; // height of window
        unsigned WindowWidth_; // width of window
        std::string ModelListPath_; // path to models
        std::string TextureListPath_; // path to textures
        std::string ShaderListPath_; // path to the shaders
        bool Fullscreen_; // do I launch in fullscreen?
        HWND HandleToWindow_; // handle to the window
        bool Profile_; // output profiling info about graphics
        bool m_renderUI; // should I render the ui?
        bool debug; // should I do debug stuff?
    };
}
