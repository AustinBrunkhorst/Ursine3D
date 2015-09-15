#pragma once

#include <d3d11.h>
#include <vector>

#include "GraphicsDefines.h"
#include "DepthStencilList.h"

namespace rey_oso
{
  class DepthStencilManager
  {
  public:
    void Initialize( ID3D11Device *device, ID3D11DeviceContext *devicecontext, int width, int height );
    void Uninitialize( );

    ID3D11DepthStencilView *GetDepthStencilView( DEPTH_STENCIL_LIST stencil );
    ID3D11ShaderResourceView *GetDepthStencilSRV( DEPTH_STENCIL_LIST stencil );
  private:
    ID3D11Device *m_device;
    ID3D11DeviceContext *m_deviceContext;

    std::vector<ID3D11DepthStencilView*> m_depthStencilViewArray;
    std::vector<ID3D11Texture2D*> m_depthStencilTextureArray;
    std::vector<ID3D11ShaderResourceView*> m_depthStencilResourceArray;

  };
}