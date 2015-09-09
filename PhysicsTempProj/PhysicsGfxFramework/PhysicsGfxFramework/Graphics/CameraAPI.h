/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      CameraAPI.h
Module:         Graphics
Purpose:        Interface for camera class
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once
#include "DLLdecl.h"
#include "GFXAPIDefines.h"
#include "Camera.h"

namespace rey_oso
{
  class CameraAPI
  {
    friend class GfxAPI;
  private:
    struct privData;

  public:
    // Create an camera
    GFX_API GFXHND AddCamera( );                          
    // get a camera
    GFX_API Camera &GetCamera( const GFXHND &handle );   
    // set active game camera
    GFX_API void SetGameCamera( const GFXHND &handle );  

  private:
    privData *m_privates;

  private:
    GFX_API void SetPrivates( void *priv );
    GFX_API void Initialize( );
    GFX_API void Uninitialize( );
  };
}