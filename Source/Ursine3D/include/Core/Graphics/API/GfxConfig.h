/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GfxConfig.h
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
    namespace graphics
    {
        struct GfxConfig
        {
            unsigned windowHeight;          // height of window
            unsigned windowWidth;           // width of window
            std::string modelListPath;      // path to models
            std::string textureListPath;    // path to textures
            std::string shaderListPath;     // path to the shaders
            bool fullscreen;                // do I launch in fullscreen?
            HWND handleToWindow;            // handle to the window
            bool enableProfiling;           // output profiling info about graphics
            bool enableDebugInfo;           // should I do debug stuff?
        };
    }
}
