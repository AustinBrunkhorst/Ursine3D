/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      RasterStateManager.h
Module:         Graphics
Purpose:        manager for the dx raster states
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/
#pragma once

#include <d3d11.h>
#include <vector>
#include "RasterStateList.h"
#include "GraphicsDefines.h"

namespace ursine
{
  namespace DXCore
  {
    class RasterStateManager
    {
    public:
      void Initialize( ID3D11Device *device, ID3D11DeviceContext *devicecontext );
      void Uninitialize( );

      ID3D11RasterizerState *GetRasterState( RASTER_STATES state );
      void SetRasterState( RASTER_STATES state );
    private:
      ID3D11Device *m_device;
      ID3D11DeviceContext *m_deviceContext;

      std::vector<ID3D11RasterizerState*> m_rasterStateArray;

      RASTER_STATES m_currentState;
    };
  }
}