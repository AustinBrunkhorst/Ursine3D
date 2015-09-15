/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      InputLayoutManager.h
Module:         Graphics
Purpose:        manager for input layouts
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once
#include <d3d11.h>
#include <vector>

#include "ShaderTypes.h"
#include "ShaderManager.h"
#include "GraphicsDefines.h"

namespace rey_oso
{
  namespace DXCore
  {
    class InputLayoutManager
    {
    public:
      void Initialize( ID3D11Device *device, ID3D11DeviceContext *devicecontext, ShaderManager *shdrmgr );
      void Uninitialize( );

      void SetInputLayout( SHADER_TYPES type );

    private:
      HRESULT GetLayoutFromBlob( Shader *shader, ID3D11InputLayout** pInputLayout );
      ID3D11Device *m_device;
      ID3D11DeviceContext *m_deviceContext;

      std::vector<ID3D11InputLayout *> m_layoutArray;

      SHADER_TYPES m_currentState;
    };
  }
}