/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      BlendStateManager.h
Module:         Graphics
Purpose:        backend for managing blend states
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/
#pragma once

#include "D3D11Forward.h"
#include <vector>
#include "BlendStateList.h"

namespace ursine
{
  namespace DXCore
  {
    class BlendStateManager
    {
    public:
      void Initialize( ID3D11Device *device, ID3D11DeviceContext *devicecontext );
      void Uninitialize( );

      ID3D11BlendState *GetBlendState( BLEND_STATES state );
      void SetBlendState( BLEND_STATES state );
    private:
      ID3D11Device *m_device;
      ID3D11DeviceContext *m_deviceContext;

      std::vector<ID3D11BlendState*> BlendStateArray_;

      BLEND_STATES m_currentState;
    };
  }
}