#include "UrsinePrecompiled.h"
#include "BlendStateManager.h"
#include <d3d11.h>

namespace ursine
{
  namespace DXCore
  {
    void BlendStateManager::Initialize( ID3D11Device *device, ID3D11DeviceContext *devicecontext )
    {
      m_device = device;
      m_deviceContext = devicecontext;

      BlendStateArray_.resize( BLEND_STATE_COUNT );

      m_currentState = BLEND_STATE_COUNT;

      ///////////////////////////////////////////////////////////////
      // NORMAL BLEND DESCRIPTION ///////////////////////////////////
      D3D11_BLEND_DESC blendDesc;
      ZeroMemory( &blendDesc, sizeof( blendDesc ) );
      blendDesc.AlphaToCoverageEnable = true;
      blendDesc.RenderTarget[ 0 ].BlendEnable = TRUE;

      blendDesc.RenderTarget[ 0 ].BlendOp = D3D11_BLEND_OP_ADD;
      blendDesc.RenderTarget[ 0 ].SrcBlend = D3D11_BLEND_SRC_ALPHA;
      blendDesc.RenderTarget[ 0 ].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

      blendDesc.RenderTarget[ 0 ].BlendOpAlpha = D3D11_BLEND_OP_ADD;
      blendDesc.RenderTarget[ 0 ].SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
      blendDesc.RenderTarget[ 0 ].DestBlendAlpha = D3D11_BLEND_ONE;

      blendDesc.RenderTarget[ 0 ].RenderTargetWriteMask = 0x0F;

      //create blend description
      m_device->CreateBlendState( &blendDesc, &BlendStateArray_[ BLEND_STATE_DEFAULT ] );

      //set blend state
      float blendFactor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };
      m_deviceContext->OMSetBlendState( BlendStateArray_[ 0 ], blendFactor, 0xffffffff );

      ///////////////////////////////////////////////////////////////
      // ADDITIVE BLEND DESCRIPTION /////////////////////////////////
      ZeroMemory( &blendDesc, sizeof( blendDesc ) );
      blendDesc.AlphaToCoverageEnable = false;
      blendDesc.RenderTarget[ 0 ].BlendEnable = TRUE;

      blendDesc.RenderTarget[ 0 ].BlendOp = D3D11_BLEND_OP_ADD;
      blendDesc.RenderTarget[ 0 ].SrcBlend = D3D11_BLEND_ONE;
      blendDesc.RenderTarget[ 0 ].DestBlend = D3D11_BLEND_ONE;

      blendDesc.RenderTarget[ 0 ].BlendOpAlpha = D3D11_BLEND_OP_ADD;
      blendDesc.RenderTarget[ 0 ].SrcBlendAlpha = D3D11_BLEND_ONE;
      blendDesc.RenderTarget[ 0 ].DestBlendAlpha = D3D11_BLEND_ONE;

      blendDesc.RenderTarget[ 0 ].RenderTargetWriteMask = 0x0F;

      //create blend description
      m_device->CreateBlendState( &blendDesc, &BlendStateArray_[ BLEND_STATE_ADDITIVE ] );

      ///////////////////////////////////////////////////////////////
      // NO BLENDING    DESCRIPTION /////////////////////////////////
      ZeroMemory( &blendDesc, sizeof( blendDesc ) );
      blendDesc.AlphaToCoverageEnable = false;
      blendDesc.RenderTarget[ 0 ].BlendEnable = TRUE;

      blendDesc.RenderTarget[ 0 ].BlendOp = D3D11_BLEND_OP_ADD;
      blendDesc.RenderTarget[ 0 ].SrcBlend = D3D11_BLEND_ONE;
      blendDesc.RenderTarget[ 0 ].DestBlend = D3D11_BLEND_ZERO;

      blendDesc.RenderTarget[ 0 ].BlendOpAlpha = D3D11_BLEND_OP_ADD;
      blendDesc.RenderTarget[ 0 ].SrcBlendAlpha = D3D11_BLEND_ONE;
      blendDesc.RenderTarget[ 0 ].DestBlendAlpha = D3D11_BLEND_ZERO;

      blendDesc.RenderTarget[ 0 ].RenderTargetWriteMask = 0x0F;

      //create blend description
      m_device->CreateBlendState( &blendDesc, &BlendStateArray_[ BLEND_STATE_NONE ] );
    }

    void BlendStateManager::Uninitialize( )
    {
      for (unsigned x = 0; x < BlendStateArray_.size( ); ++x)
      {
        if (BlendStateArray_[ x ] != NULL)
        {
          BlendStateArray_[ x ]->Release( );
          BlendStateArray_[ x ] = NULL;
        }
      }

      m_device = NULL;
      m_deviceContext = NULL;
    }

    ID3D11BlendState *BlendStateManager::GetBlendState( BLEND_STATES state )
    {
      return BlendStateArray_[ state ];
    }

    void BlendStateManager::SetBlendState( BLEND_STATES state )
    {
      if (m_currentState == state)
        return;

      m_currentState = state;

      float blendFactor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };
      m_deviceContext->OMSetBlendState( BlendStateArray_[ state ], blendFactor, 0xffffffff );
    }

    void BlendStateManager::Invalidate ( )
    {
      m_currentState = BLEND_STATE_COUNT;
    }
  }
}