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

#include <d3d11.h>
#include <Color.h>
#include <SMat4.h>

#include "GraphicsDefines.h"
#include "ViewportRenderModes.h"

namespace ursine
{
  class Viewport
  {
  public:
    // initialize viewport to default settings
    void Initialize( );

    // set background color of this viewport
    void SetBackgroundColor( float r, float g, float b, float a );
    void SetBackgroundColor( Color color );

    // get the background, array of 4 floats
    const float *GetBackgroundColor( );

    // set the camera for this viewport
    void SetViewportCamera( GFXHND camera );

    // get the camera for this viewport
    GFXHND GetViewportCamera( );

    //set render mode (orthographics VS perspective)
    void SetRenderMode( ViewportRenderMode renderMode );

    //get the render mode
    ViewportRenderMode GetRenderMode( );

    // set dimensions of the viewport, in pixels
    void SetDimensions( unsigned width, unsigned height );

    // set the top-left position of this viewport, in pixels. 
    // top left is 0, 0, bottom right is screenWidth, screenHeight
    void SetPosition( unsigned positionX, unsigned positionY );

    // get directx viewport data
    D3D11_VIEWPORT GetViewportData( );
  private:
    float m_color[ 4 ];               // background color

    GFXHND m_camera;                  // camera handle

    ViewportRenderMode m_renderMode;  // render in forward or deferred mode?

    unsigned m_width;                 // dimension x
    unsigned m_height;                // dimension y

    unsigned m_positionX;             // position x
    unsigned m_positionY;             // position y

    unsigned m_rt;                    // current render target to use
  };
}
