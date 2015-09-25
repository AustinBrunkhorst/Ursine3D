/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      CameraManager.h
Module:         Graphics
Purpose:        manager of cameras and handles
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include <vector>

#include "GraphicsDefines.h"
#include "Camera.h"

namespace ursine
{
  class CameraManager
  {
  public:
    void Initialize( );
    void Uninitialize( );

    GFXHND CreateCamera( );
    Camera &GetCamera( const GFXHND &handle );

    Camera &GetCurrent( );
    void SetCurrent( const GFXHND &handle );
  private:
    std::vector<Camera> m_cameraArray;
    unsigned m_current;
  };
}