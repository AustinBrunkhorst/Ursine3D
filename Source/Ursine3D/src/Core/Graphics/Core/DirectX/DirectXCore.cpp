#include "UrsinePrecompiled.h"
#include "DirectXCore.h"

namespace ursine
{
  namespace DXCore
  {
    void DirectXCore::Initialize( const unsigned width, const unsigned height, HWND *hWindow, ursine::GfxInfo *gfxInfo, bool fullscreen )
    {
      //allocate all
      m_blendManager = new BlendStateManager;
      m_depthStateManager = new DepthStateManager;
      m_targetManager = new RenderTargetManager;
      m_rasterStateManager = new RasterStateManager;
      m_depthStencilManager = new DepthStencilManager;

      //initializing
      HRESULT result;

      m_swapChain = NULL;
      m_deviceContext = NULL;
      m_device = NULL;

      /////////////////////////////////////////////////////////////////
      // GET REFRESH RATE /////////////////////////////////////////////
      unsigned numerator, denominator;

      for (unsigned x = 0; x < gfxInfo->GetModeCount( ); ++x)
      {
        if (gfxInfo->GetDisplayInfo( x ).Width == width)
        {
          if (gfxInfo->GetDisplayInfo( x ).Height == height)
          {
            numerator = gfxInfo->GetDisplayInfo( x ).RefreshRate.Numerator;
            denominator = gfxInfo->GetDisplayInfo( x ).RefreshRate.Denominator;
          }
        }
      }

      /////////////////////////////////////////////////////////////////
      // CREATING SWAP CHAIN //////////////////////////////////////////
      DXGI_SWAP_CHAIN_DESC swapChainDesc;
      D3D_FEATURE_LEVEL featureLevel;
      ID3D11Texture2D *backBufferPtr;

      D3D_FEATURE_LEVEL FeatureLevelArray[ 10 ];
      FeatureLevelArray[ 0 ] = D3D_FEATURE_LEVEL_11_1;
      FeatureLevelArray[ 1 ] = D3D_FEATURE_LEVEL_11_0;
      FeatureLevelArray[ 2 ] = D3D_FEATURE_LEVEL_10_1;
      FeatureLevelArray[ 3 ] = D3D_FEATURE_LEVEL_10_0;

      //Initialize the swap chain description.
      ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );

      //Set to a single back buffer.
      swapChainDesc.BufferCount = 1;

      //Set the width and height of the back buffer.
      swapChainDesc.BufferDesc.Width = width;
      swapChainDesc.BufferDesc.Height = height;

      //Set regular 32-bit surface for the back buffer.
      swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

      //Set the refresh rate of the back buffer.
      if (false) //@Matt change this to properly do vsync
      {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
      }
      else
      {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
      }

      //Set the usage of the back buffer.
      swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

      //Set the handle for the window to render to.
      swapChainDesc.OutputWindow = *hWindow;

      //set multisampling
      swapChainDesc.SampleDesc.Count = gfxInfo->GetSampleCount( );
      swapChainDesc.SampleDesc.Quality = gfxInfo->GetSampleQuality( );

      LogMessage( "Sample Count: %i", 2, swapChainDesc.SampleDesc.Count );
      LogMessage( "Sample Quality: %i", 2, swapChainDesc.SampleDesc.Quality );

      //set swap chain flags
      swapChainDesc.Windowed = true; //@Mat rekt
      swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

      //Set the scan line ordering and scaling to unspecified.
      swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
      swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

      //Discard the back buffer contents after presenting.
      swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

      //Set the feature level to DirectX 11.
      featureLevel = D3D_FEATURE_LEVEL_11_1;

      //Create the swap chain, Direct3D device, and Direct3D device context.
#ifdef _DEBUG
      result = D3D11CreateDeviceAndSwapChain( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
        D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_DEBUG, FeatureLevelArray, 4, D3D11_SDK_VERSION,
        &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext );
      UAssert( result == S_OK, "Failed to make device and swap chain! (Error '0x%x')", result );
#else
      result = D3D11CreateDeviceAndSwapChain( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
        D3D11_CREATE_DEVICE_SINGLETHREADED, &featureLevel, 1, D3D11_SDK_VERSION,
        &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext );
      UAssert( result == S_OK, "Failed to make device and swap chain! (Error '0x%x')", result );
#endif

      D3D_FEATURE_LEVEL finalFeatureLevel = m_device->GetFeatureLevel( );
      LogMessage( "Feature Level: %i", 2, finalFeatureLevel );

      //init targets
      m_targetManager->Initialize( m_device, m_deviceContext );
      m_targetManager->CreateTargets( );

      //set to not fullscreen
      m_swapChain->SetFullscreenState( fullscreen, NULL );

      //Get the pointer to the back buffer.
      result = m_swapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr );
      UAssert( result == S_OK, "Failed to get back buffer! (Error '0x%x')", result );

      //Create the render target view with the back buffer pointer.
      result = m_device->CreateRenderTargetView( backBufferPtr, NULL,
        &m_targetManager->GetRenderTarget( RENDER_TARGET_SWAPCHAIN )->RenderTargetView );
      UAssert( result == S_OK, "Failed to make render target! (Error '%i')", result );

      //Release pointer to the back buffer as we no longer need it.
      backBufferPtr->Release( );
      backBufferPtr = NULL;

      /////////////////////////////////////////////////////////////////
      // CREATING DEPTH STATES ////////////////////////////////////////
      m_depthStateManager->Initialize( m_device, m_deviceContext );

      /////////////////////////////////////////////////////////////////
      // CREATING DEPTH VIEW //////////////////////////////////////////
      m_depthStencilManager->Initialize( m_device, m_deviceContext, width, height );

      /////////////////////////////////////////////////////////////////
      // SET MAIN RENDER TARGET ///////////////////////////////////////
      m_deviceContext->OMSetRenderTargets( 1, &m_targetManager->GetRenderTarget( RENDER_TARGET_SWAPCHAIN )->RenderTargetView, m_depthStencilManager->GetDepthStencilView(DEPTH_STENCIL_MAIN) );

      /////////////////////////////////////////////////////////////////
      // CREATING RASTER STATES ///////////////////////////////////////
      m_rasterStateManager->Initialize( m_device, m_deviceContext );
      m_rasterStateManager->SetRasterState( RASTER_STATE_BACKFACE_CULL );

      /////////////////////////////////////////////////////////////////
      // CREATING BLEND STATES ////////////////////////////////////////
      m_blendManager->Initialize( m_device, m_deviceContext );

      /////////////////////////////////////////////////////////////////
      // CREATING VIEWPORT ////////////////////////////////////////////
      D3D11_VIEWPORT viewport;
      ZeroMemory( &viewport, sizeof( viewport ) );

      viewport.Width = static_cast<float>(width);
      viewport.Height = static_cast<float>(height);
      viewport.MinDepth = 0.0f;
      viewport.MaxDepth = 1.0f;
      viewport.TopLeftX = 0.0f;
      viewport.TopLeftY = 0.0f;

      m_deviceContext->RSSetViewports( 1, &viewport );

      /////////////////////////////////////////////////////////////////
      // CREATE OTHER RENDER TARGETS //////////////////////////////////
      m_targetManager->InitializeAllRenderTargets( width, height );
    }

    void DirectXCore::Uninitialize( )
    {
      //set to not fullscreen
      m_swapChain->SetFullscreenState( false, NULL );

      m_rasterStateManager->Uninitialize( );
      m_blendManager->Uninitialize( );
      m_depthStateManager->Uninitialize( );
      m_targetManager->Uninitialize( );
      m_depthStencilManager->Uninitialize( );

      delete m_blendManager;
      delete m_depthStateManager;
      delete m_targetManager;
      delete m_depthStencilManager;

      RELEASE_RESOURCE( m_swapChain );
      RELEASE_RESOURCE( m_deviceContext );
      RELEASE_RESOURCE( m_device );
    }

    void DirectXCore::ClearDeferredBuffers( )
    {
      float color[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };
      float clear[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
      float colorNormal[ 4 ] = { 0.5, 0.5, 0.5, 1.0f };
      float colorDepth[ 4 ] = { 1, 1, 1, 1.0f };

      float lightMap[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
      m_deviceContext->ClearRenderTargetView( m_targetManager->GetRenderTarget( RENDER_TARGET_LIGHTMAP )->RenderTargetView, lightMap );
      m_deviceContext->ClearRenderTargetView( m_targetManager->GetRenderTarget( RENDER_TARGET_DEFERRED_COLOR )->RenderTargetView, color );
      m_deviceContext->ClearDepthStencilView( m_depthStencilManager->GetDepthStencilView(DEPTH_STENCIL_MAIN), D3D11_CLEAR_DEPTH, 1.0f, 0 );
      m_deviceContext->ClearRenderTargetView( m_targetManager->GetRenderTarget( RENDER_TARGET_DEFERRED_DEPTH )->RenderTargetView, colorNormal );
      m_deviceContext->ClearRenderTargetView( m_targetManager->GetRenderTarget( RENDER_TARGET_DEFERRED_NORMAL )->RenderTargetView, color );
      m_deviceContext->ClearRenderTargetView( m_targetManager->GetRenderTarget( RENDER_TARGET_DEBUG )->RenderTargetView, clear );
    }

    void DirectXCore::ClearBuffers( )
    {
      float color[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };

      m_deviceContext->ClearRenderTargetView( m_targetManager->GetRenderTarget( RENDER_TARGET_SWAPCHAIN )->RenderTargetView, color );

      //clear other targets
      for (int x = RENDER_TARGET_VIEWPORT_1; x < RENDER_TARGET_COUNT; ++x)
      {
        if (m_targetManager->GetRenderTarget( (RENDER_TARGETS)x )->RenderTargetView == NULL)
          continue;

        m_deviceContext->ClearRenderTargetView( m_targetManager->GetRenderTarget( (RENDER_TARGETS)x )->RenderTargetView, color );
      }

    }

    void DirectXCore::SwapChainBuffer( )
    {
      m_swapChain->Present( 1, 0 );
    }

    ID3D11Device *DirectXCore::GetDevice( )
    {
      return m_device;
    }

    ID3D11DeviceContext *DirectXCore::GetDeviceContext( )
    {
      return m_deviceContext;
    }

    //set blend
    void DirectXCore::SetBlendState( BLEND_STATES bt )
    {
      m_blendManager->SetBlendState( bt );
    }
    //set depth stencil
    void DirectXCore::SetDepthState( DEPTH_STATES dt )
    {
      m_depthStateManager->SetDepthState( dt );
    }
    //set target
    void DirectXCore::SetRenderTarget( RENDER_TARGETS rt )
    {
      m_targetManager->SetRenderTarget( rt, m_depthStencilManager->GetDepthStencilView(DEPTH_STENCIL_MAIN) );
    }

    void DirectXCore::SetRasterState( RASTER_STATES state )
    {
      m_rasterStateManager->SetRasterState( state );
    }

    //get blend
    BlendStateManager *DirectXCore::GetBlendMgr( )
    {
      return m_blendManager;
    }
    //get depth stencil
    DepthStateManager *DirectXCore::GetDepthMgr( )
    {
      return m_depthStateManager;
    }
    //get target
    RenderTargetManager *DirectXCore::GetRenderTargetMgr( )
    {
      return m_targetManager;
    }

    DepthStencilManager* DirectXCore::GetDepthStencilMgr ( )
    {
      return m_depthStencilManager;
    }
  }
}