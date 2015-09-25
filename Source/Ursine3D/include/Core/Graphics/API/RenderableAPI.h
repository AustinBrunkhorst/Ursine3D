/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      RenderableAPI.h
Module:         Graphics
Purpose:        API for managing renderables
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include "GFXAPIDefines.h"
#include "Renderable.h"

namespace ursine
{
  class RenderableAPI
  {
    friend class GfxAPI;
  private:
    struct privData;

  public:
    // create a graphics object
    GFXHND AddRenderable( const RENDERABLE_TYPE type );   

    // destroy graphics object
    void DestroyRenderable( GFXHND &handle );  

    // get a model
    Model3D &GetModel3D( GFXHND &handle );    

    // get a directional light
    DirectionalLight &GetDirectionalLight( GFXHND &handle );  

    // get a point light
    PointLight &GetPointLight( GFXHND &handle );  

    // get a point light
    Primitive &GetPrimitive( GFXHND &handle );                

  private:
    privData *m_privates;

  private:
    void SetPrivates( void *priv );
    void Initialize( );
    void Uninitialize( );

  };
}