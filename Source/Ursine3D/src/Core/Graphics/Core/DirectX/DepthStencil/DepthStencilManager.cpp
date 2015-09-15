#include "UrsinePrecompiled.h"

#include "DepthStencilManager.h"

namespace ursine
{
  namespace DXCore
  {
    void DepthStencilManager::Initialize( ID3D11Device *device, ID3D11DeviceContext *devicecontext )
    {
      m_device = device;
      m_deviceContext = devicecontext;

      m_depthStateArray.resize( DEPTH_STATE_COUNT );

      m_currentState = DEPTH_STATE_COUNT;

      /////////////////////////////////////////////////////////////////
      // DEPTH CHECKING ///////////////////////////////////////////////
      HRESULT result;
      D3D11_DEPTH_STENCIL_DESC depthCheckStencilDesc;
      ZeroMemory( &depthCheckStencilDesc, sizeof( depthCheckStencilDesc ) );

      //Set up the description of the stencil state.
      depthCheckStencilDesc.DepthEnable = true;
      depthCheckStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
      depthCheckStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

      depthCheckStencilDesc.StencilEnable = true;
      depthCheckStencilDesc.StencilReadMask = 0xFF;
      depthCheckStencilDesc.StencilWriteMask = 0xFF;

      //Stencil operations if pixel is front-facing.
      depthCheckStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
      depthCheckStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
      depthCheckStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
      depthCheckStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

      //Stencil operations if pixel is back-facing.
      depthCheckStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
      depthCheckStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
      depthCheckStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
      depthCheckStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

      //Create the depth stencil state.
      result = m_device->CreateDepthStencilState( &depthCheckStencilDesc, &m_depthStateArray[ DEPTH_STATE_DEPTH_CHECK ] );
      UAssert( result == S_OK, "Failed to create depth stencil! (Error '%i')", result );

      /////////////////////////////////////////////////////////////////
      // NO DEPTH CHECKING ////////////////////////////////////////////
      ZeroMemory( &depthCheckStencilDesc, sizeof( depthCheckStencilDesc ) );

      //Set up the description of the stencil state.
      depthCheckStencilDesc.DepthEnable = false;
      depthCheckStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
      depthCheckStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

      depthCheckStencilDesc.StencilEnable = true;
      depthCheckStencilDesc.StencilReadMask = 0xFF;
      depthCheckStencilDesc.StencilWriteMask = 0xFF;

      //Stencil operations if pixel is front-facing.
      depthCheckStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
      depthCheckStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
      depthCheckStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
      depthCheckStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

      //Stencil operations if pixel is back-facing.
      depthCheckStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
      depthCheckStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
      depthCheckStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
      depthCheckStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

      //Create the depth stencil state.
      result = m_device->CreateDepthStencilState( &depthCheckStencilDesc, &m_depthStateArray[ DEPTH_STATE_NO_DEPTH_CHECK ] );
      UAssert( result == S_OK, "Failed to create depth stencil! (Error '%i')", result );
    }

    void DepthStencilManager::Uninitialize( )
    {
      for (unsigned x = 0; x < m_depthStateArray.size( ); ++x)
      {
        RELEASE_RESOURCE( m_depthStateArray[ x ] );
      }

      m_device = NULL;
      m_deviceContext = NULL;
    }

    ID3D11DepthStencilState *DepthStencilManager::GetStencilState( DEPTH_STATES state )
    {
      return m_depthStateArray[ state ];
    }

    void DepthStencilManager::SetDepthState( DEPTH_STATES state )
    {
      if (m_currentState == state)
        return;

      m_currentState = state;

      m_deviceContext->OMSetDepthStencilState( m_depthStateArray[ state ], 1 );
    }
  }
}