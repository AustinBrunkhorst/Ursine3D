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


namespace ursine
{
  class DepthStateManager
  {
  public:
    void Initialize( ID3D11Device *device, ID3D11DeviceContext *devicecontext, int width, int height );
    void Uninitialize( );

    ID3D11DepthStencilView *GetDepthStencilView( DEPTH_STENCIL_LIST stencil );
    ID3D11ShaderResourceView *GetDepthStencilSRV( DEPTH_STENCIL_LIST stencil );

    void Resize( int width, int height );
  private:
    ID3D11Device *m_device;
    ID3D11DeviceContext *m_deviceContext;

    std::vector<ID3D11DepthStencilView*> m_depthStencilViewArray;
    std::vector<ID3D11Texture2D*> m_depthStencilTextureArray;
    std::vector<ID3D11ShaderResourceView*> m_depthStencilResourceArray;

  };
}

