/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      DepthStateManager.h
Module:         Graphics
Purpose:        manager for handling depth states
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/
#pragma once

#include <d3d11.h>
#include <vector>
#include "DepthStateList.h"
#include "GraphicsDefines.h"

namespace rey_oso
{
  namespace DXCore
  {
    class DepthStateManager
    {
    public:
      void Initialize( ID3D11Device *device, ID3D11DeviceContext *devicecontext );
      void Uninitialize( );

      ID3D11DepthStencilState *GetStencilState( DEPTH_STATES state );
      void SetDepthState( DEPTH_STATES state );
    private:
      ID3D11Device *m_device;
      ID3D11DeviceContext *m_deviceContext;

      std::vector<ID3D11DepthStencilState*> m_depthStateArray;

      DEPTH_STATES m_currentState;
    };
  }
}