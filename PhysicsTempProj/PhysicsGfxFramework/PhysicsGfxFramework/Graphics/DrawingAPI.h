/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      DrawingAPI.h
Module:         Graphics
Purpose:        API for rendering points and lines in the world
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include "DLLdecl.h"

#include "GFXAPIDefines.h"

namespace rey_oso
{
  class DrawingAPI
  {
    friend class GfxAPI;
  private:
    struct privData;

  public:
    // draw a point
    void DrawPoint( float x, float y, float z );      
    // draw a line 
    void DrawLine( float x0, float y0, float z0, float x1, float y1, float z1 );  
                                       
    // set render color
    void SetColor( float r, float g, float b, float a );        

    // size of the points, in pixels
    void SetSize( float size );                                                   
  private:
    privData *m_privates;

  private:
    GFX_API void SetPrivates( void *priv );
    GFX_API void Initialize( );
    GFX_API void Uninitialize( );
  };
}