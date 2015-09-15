/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      DirectXCore.h
Module:         Graphics
Purpose:        Core dx class for managing all dx resources
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/
#pragma once

#include <d3d11.h>
#include <Windows.h>
#include "GfxInfo.h"
#include "BlendStateManager.h"
#include "DepthStateManager.h"
#include "RenderTargetManager.h"
#include "RasterStateManager.h"
#include "DepthStencilManager.h"

namespace ursine
{
  namespace DXCore
  {
    class DirectXCore
    {
    public:
      //initialize everything
      void Initialize( const unsigned width, const unsigned height, HWND *hWindow, ursine::GfxInfo *gfxInfo, bool fullscreen );
      void Uninitialize( );

      //methods
      //clears buffers
      void ClearDeferredBuffers( );
      void ClearBuffers( );

      //swaps the swapchain
      void SwapChainBuffer( );

      //get the device
      ID3D11Device *GetDevice( );

      //get the devcon
      ID3D11DeviceContext *GetDeviceContext( );

      //set blend
      void SetBlendState( BLEND_STATES bt );
      //set depth stencil
      void SetDepthState( DEPTH_STATES dt );
      //set target
      void SetRenderTarget( RENDER_TARGETS rt );

      //set raster state
      void SetRasterState( RASTER_STATES state );

      //get blend
      BlendStateManager *GetBlendMgr( );
      //get depth stencil
      DepthStateManager *GetDepthMgr( );
      //get target
      RenderTargetManager *GetRenderTargetMgr( );
      //get stencil manager
      DepthStencilManager *GetDepthStencilMgr( );

      void ResizeDX( int width, int height );

    private:
      ID3D11Device *m_device;
      ID3D11DeviceContext *m_deviceContext;
      IDXGISwapChain *m_swapChain;

      BlendStateManager *m_blendManager;
      DepthStateManager *m_depthStateManager;
      DepthStencilManager *m_depthStencilManager;
      RenderTargetManager *m_targetManager;
      RasterStateManager *m_rasterStateManager;
    };
  }
}