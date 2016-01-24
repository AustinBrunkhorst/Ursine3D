/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Viewport.h
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
File Name:      Viewport.h
Module:         Graphics
Purpose:        Viewport class
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include "D3D11Forward.h"
#include "GfxDefines.h"
#include "ViewportRenderModes.h"

namespace ursine
{
    namespace graphics
    {
        class Viewport
        {
        public:
            // initialize viewport to default settings
            void Initialize();

            // set background color of this viewport
            void SetBackgroundColor(float r, float g, float b, float a);
            void SetBackgroundColor(Color color);

            // get the background, array of 4 floats
            const float *GetBackgroundColor();

            void GetDimensions(unsigned &width, unsigned &height);

            // set dimensions of the viewport, in pixels
            void SetDimensions(unsigned width, unsigned height);

            void GetPosition(unsigned &positionX, unsigned &positionY);

            // set the top-left position of this viewport, in pixels. 
            // top left is 0, 0, bottom right is screenWidth, screenHeight
            void SetPosition(unsigned positionX, unsigned positionY);

            // get directx viewport data
            D3D11_VIEWPORT GetViewportData();
        private:
            float m_color[ 4 ]; // background color

            unsigned m_width; // dimension x
            unsigned m_height; // dimension y

            unsigned m_positionX; // position x
            unsigned m_positionY; // position y
        };
    }
}
