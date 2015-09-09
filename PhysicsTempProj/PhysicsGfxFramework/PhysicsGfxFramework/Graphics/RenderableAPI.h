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
#include "DLLdecl.h"
#include "GFXAPIDefines.h"
#include "Renderable.h"

namespace rey_oso
{
  class RenderableAPI
  {
    friend class GfxAPI;
  private:
    struct privData;

  public:
    // create a graphics object
    GFX_API GFXHND AddRenderable( const RENDERABLE_TYPE type );   

    // destroy graphics object
    GFX_API void DestroyRenderable( GFXHND &handle );  

    // get a model
    GFX_API Model3D &GetModel3D( GFXHND &handle );    

    // get a directional light
    GFX_API DirectionalLight &GetDirectionalLight( GFXHND &handle );  

    // get a point light
    GFX_API PointLight &GetPointLight( GFXHND &handle );  

    // get a point light
    GFX_API Primitive &GetPrimitive( GFXHND &handle );                

  private:
    privData *m_privates;

  private:
    GFX_API void SetPrivates( void *priv );
    GFX_API void Initialize( );
    GFX_API void Uninitialize( );

  };
}