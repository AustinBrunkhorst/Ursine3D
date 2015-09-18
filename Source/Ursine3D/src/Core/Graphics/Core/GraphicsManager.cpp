#include "UrsinePrecompiled.h"
#include "GraphicsManager.h"

//@Matt
#include "VertexDefinitions.h"
#include <complex>

namespace ursine
{
  bool sort( DRAWHND &h1, DRAWHND &h2 )
  {
    if (*reinterpret_cast<unsigned long long*>(&h1) < *reinterpret_cast<unsigned long long*>(&h2))
      return true;
    return false;
  }

  void GraphicsCore::Initialize( GfxConfig &config )
  {
    /////////////////////////////////////////////////////////////////
    // ALLOCATE MANAGERS ////////////////////////////////////////////
    dxCore = new DXCore::DirectXCore;
    gfxInfo = new GfxInfo;
    shaderManager = new DXCore::ShaderManager;
    bufferManager = new DXCore::ShaderBufferManager;
    layoutManager = new DXCore::InputLayoutManager;
    modelManager = new ModelManager;
    renderableManager = new RenderableManager;
    cameraManager = new CameraManager;
    textureManager = new TextureManager;
    viewportManager = new ViewportManager;
    uiManager = new GfxUIManager;
    drawingManager = new DrawingManager;
    gfxProfiler = new GfxProfiler;

    /////////////////////////////////////////////////////////////////
    // INITIALIZING /////////////////////////////////////////////////
    m_drawList.resize( MAX_DRAW_CALLS );
    m_drawCount = 0;
    m_renderUI = config.m_renderUI;
    m_profile = config.Profile_;
    m_threadHandle = nullptr;
    m_debug = config.debug;

    //writing log stuff
    LogMessage( "GRAPHICS" );
    LogMessage( "Graphics Config:", 1 );
    LogMessage( "Width: %i", 2, config.WindowWidth_ );
    LogMessage( "Height: %i", 2, config.WindowHeight_ );
    LogMessage( "Model Path: %s", 2, config.ModelListPath_.c_str( ) );
    LogMessage( "Texture Path: %s", 2, config.TextureListPath_.c_str( ) );
    LogMessage( "Shader Path: %s", 2, config.ShaderListPath_.c_str( ) );
    LogMessage( "Fullscreen: %s", 2, config.Fullscreen_ == true ? "True" : "False" );
    LogMessage( "GPU Info", 1 );
    gfxInfo->Initialize( );
    gfxInfo->SetDimensions( config.WindowWidth_, config.WindowHeight_ );

    /////////////////////////////////////////////////////////////////
    // INITIALIZE MANAGERS //////////////////////////////////////////

    LogMessage( "Initialize DirectX", 1 );
    dxCore->Initialize( config.WindowWidth_, config.WindowHeight_, config.HandleToWindow_, gfxInfo, config.Fullscreen_, m_debug );

    LogMessage( "Initialize Shaders", 1 );
    shaderManager->Initialize( dxCore->GetDevice( ), dxCore->GetDeviceContext( ), config.ShaderListPath_ );

    {
      //load shaders
      shaderManager->LoadShader( SHADER_BASIC, "BasicModelShader" );
      shaderManager->LoadShader( SHADER_DIFFUSE, "DiffuseShader" );
      shaderManager->LoadShader( SHADER_NORMAL, "NormalShader" );
      shaderManager->LoadShader( SHADER_DEFFERED_TEXTURE, "DeferredTextureShader" );
      shaderManager->LoadShader( SHADER_DEFERRED_DEPTH, "DeferredDepth" );
      shaderManager->LoadShader( SHADER_DEFERRED_DEPTH_NORM, "DeferredDepthNormalMap" );
      shaderManager->LoadShader( SHADER_DIRECTIONAL_LIGHT, "DirectionalLightSource" );
      shaderManager->LoadShader( SHADER_POINT_LIGHT, "PointLightSource" );
      shaderManager->LoadShader( SHADER_QUAD, "QuadShader" );
      shaderManager->LoadShader( SHADER_UI, "UIShader" );
      shaderManager->LoadShader( SHADER_PRIMITIVE, "PrimitiveShader" );
      shaderManager->LoadShader( SHADER_POINT, "PointShader" );
      shaderManager->LoadShader( SHADER_SHADOW, "ShadowMap" );
    }

    LogMessage( "Initialize Buffers", 1 );
    bufferManager->Initialize( dxCore->GetDevice( ), dxCore->GetDeviceContext( ) );
    LogMessage( "Initialize Models", 1 );
    modelManager->Initialize( dxCore->GetDevice( ), dxCore->GetDeviceContext( ), config.ModelListPath_ );
    renderableManager->Initialize( );
    cameraManager->Initialize( );
    LogMessage( "Initialize Textures", 1 );
    textureManager->Initialize( dxCore->GetDevice( ), dxCore->GetDeviceContext( ), config.TextureListPath_ );
    viewportManager->Initialize( dxCore->GetRenderTargetMgr( ) );
    uiManager->Initialize( dxCore->GetDevice( ), dxCore->GetDeviceContext( ), dxCore->GetRenderTargetMgr( ), this );
    drawingManager->Initialize( dxCore->GetDevice( ), dxCore->GetDeviceContext( ) );
    gfxProfiler->Initialize( dxCore->GetDevice( ), dxCore->GetDeviceContext( ), m_profile );

    //create layouts
    layoutManager->Initialize( dxCore->GetDevice( ), dxCore->GetDeviceContext( ), shaderManager );

    //init drawing manager
    drawingManager->EndScene( );

    m_ready = true;
  }

  void GraphicsCore::Uninitialize( )
  {
    //WaitForSingleObject( m_threadHandle, INFINITE );

    gfxInfo->Uninitialize( );
    shaderManager->Uninitialize( );
    bufferManager->Uninitialize( );
    layoutManager->Uninitialize( );
    modelManager->Uninitialize( );
    renderableManager->Uninitialize( );
    cameraManager->Uninitialize( );
    textureManager->Uninitialize( );
    viewportManager->Uninitialize( );
    uiManager->Uninitialize( );
    drawingManager->Uninitialize( );
    gfxProfiler->Uninitialize( );

    //last
    dxCore->Uninitialize( );

    delete gfxInfo;
    delete dxCore;
    delete shaderManager;
    delete bufferManager;
    delete layoutManager;
    delete modelManager;
  }

  void GraphicsCore::Render( GFXHND handle )
  {
    //convert to renderable handle
    _RENDERABLEHND *render = HND_RENDER( handle );

    //make sure right handle was passed
    UAssert( render->ID_ == ID_RENDERABLE, "Attempted to draw non-valid handle!" );

    //make sure we have enough room to render
    UAssert( m_drawCount < MAX_DRAW_CALLS, "Out of available draw calls! Let Matt know, easy fix." );

    //get a new draw call
    DRAWHND &drawCall = m_drawList[ m_drawCount++ ];

    switch (render->Type_)
    {
      //rendering a model
    case RENDERABLE_MODEL3D:
    {
      Model3D *current = &renderableManager->m_renderableModel3D[ render->Index_ ];

      drawCall.Index_ = render->Index_;
      drawCall.Type_ = render->Type_;
      drawCall.Material_ = textureManager->GetTextureIDByName( current->GetMaterialslName( ) );

      drawCall.Model_ = modelManager->GetModelIDByName( current->GetModelName( ) );
      drawCall.Shader_ = SHADER_DEFERRED_DEPTH;
    }
    break;
    //directional light
    case RENDERABLE_DIRECTION_LIGHT:
    {
      DirectionalLight *current = &renderableManager->m_renderableDirectionalLight[ render->Index_ ];

      drawCall.Index_ = render->Index_;
      drawCall.Type_ = render->Type_;

      drawCall.Shader_ = SHADER_DIRECTIONAL_LIGHT;
    }
    break;
    //point light
    case RENDERABLE_POINT_LIGHT:
    {
      PointLight *current = &renderableManager->m_renderablePointLight[ render->Index_ ];

      drawCall.Index_ = render->Index_;
      drawCall.Type_ = render->Type_;

      drawCall.Shader_ = SHADER_POINT_LIGHT;
    }
    break;
    case RENDERABLE_PRIMITIVE:
    {
      Primitive *current = &renderableManager->m_renderablePrimitives[ render->Index_ ];

      drawCall.Index_ = render->Index_;
      drawCall.Type_ = render->Type_;

      drawCall.Shader_ = SHADER_PRIMITIVE;
      drawCall.debug_ = 1;
    }
    break;
    default:
      break;
    }
  }

  void GraphicsCore::BeginScene( )
  {
    while (m_rendering);

    m_rendering = true;

    dxCore->ClearBuffers( );
    gfxProfiler->BeginFrame( );
  }

  void GraphicsCore::RenderScene( float dt, GFXHND viewport )
  {
	  RenderScene_Deferred(dt, viewport);
	  return;
    //close thread handle if needed
    if (m_threadHandle != nullptr)
      CloseHandle( m_threadHandle );
    RenderScene_Deferred( dt, viewport );

    return;
    /*
    auto *data = new threadData;
    data->gfx = this;
    data->dt = dt;
    data->forward = viewportManager->GetRenderMode( viewport ) == VIEWPORT_RENDER_FORWARD;
    data->viewport = viewport;

    m_threadHandle = CreateThread( nullptr, 0, renderBootstrap, data, 0, &m_threadID );*/
  }

  DWORD GraphicsCore::renderBootstrap( LPVOID lpParam )
  {
    auto *data = reinterpret_cast<threadData*>(lpParam);

    if (data->forward == true)
    {
      data->gfx->RenderScene_Forward( data->dt, data->viewport );
    }
    else
    {
      data->gfx->RenderScene_Deferred( data->dt, data->viewport );
    }

    data->gfx->EndScene( );

    delete lpParam;

    return 0;
  }

  void GraphicsCore::RenderScene_Forward( float dt, GFXHND viewport )
  {
    //// init buffers for frame
    //dxCore->ClearDeferredBuffers( );
    //gfxProfiler->Stamp( PROFILE_CLEAR_BUFFERS );

    //// PRE FRAME STUFF //////////////////////////////////////////////
    ////get camera
    ////Camera &currentCamera = cameraManager->GetCamera( viewportManager->GetCamera( viewport ) );
    ////get viewport handle
    //_VIEWPORTHND *vpHandle = reinterpret_cast<_VIEWPORTHND*>(&viewport);
    //unsigned wwidth, wheight;
    //int wposX, wposY;

    ////calculate stuff for viewport
    //unsigned w, h;
    //gfxInfo->GetDimensions( w, h );

    //float posX = wposX * 2.f / w;   //position of the viewport
    //float posY = wposY * 2.f / h;

    //float width = wwidth / static_cast<float>(w);       //actual ratio of viewport to window
    //float height = wheight / static_cast<float>(h);

    //DirectX::XMMATRIX proj;
    //if (currentCamera.GetProjMode( ) == Camera::ProjectionMode::PROJECTION_PERSPECTIVE)
    //{
    //  proj = DirectX::XMMatrixPerspectiveFovLH( currentCamera.GetFOV( ) * 3.14f / 180.f, (float)wwidth / wheight, 0.0001f, 1000.f );
    //}
    //else
    //{
    //  proj = DirectX::XMMatrixOrthographicLH( (float)wwidth / 100, (float)wheight / 100, 0.0001f, 1000.f );
    //}
    ////check stuff, get things ready to go
    //RENDER_TARGETS mainTarget = (RENDER_TARGETS)(viewportManager->GetRenderTargetID( viewport ) + RENDER_TARGET_VIEWPORT_1);

    //// raster state

    ////set all states
    //dxCore->SetBlendState( BLEND_STATE_DEFAULT );
    //dxCore->SetDepthState( DEPTH_STATE_DEPTH_CHECK );

    ////forward shading
    //shaderManager->BindShader( SHADER_DIFFUSE );
    //layoutManager->SetInputLayout( SHADER_DIFFUSE );

    ////set render type
    //dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    ////map the sampler
    //textureManager->MapSamplerState( SAMPLER_WRAP_TEX );

    ////set culling
    //dxCore->SetRasterState( RASTER_STATE_BACKFACE_CULL );

    //dxCore->SetRenderTarget( mainTarget );

    ///////////////////////////////////////////////////////////////////
    //// TEMPORARY 

    ////map camera and proj
    //bufferManager->MapCameraBuffer( currentCamera.GetViewMatrix( ), proj );

    //// SORT ALL DRAW CALLS
    //std::sort( m_drawList.begin( ), m_drawList.begin( ) + m_drawCount, sort );

    ////prepping for light render and stuff
    //bool lightStart = false;
    //bool debugStart = false;

    //// calls all draw calls
    //for (unsigned x = 0; x < m_drawCount; ++x)
    //{
    //  //how to render the current object?
    //  switch (m_drawList[ x ].Type_)
    //  {
    //    //rendering a model
    //  case RENDERABLE_MODEL3D:
    //  {
    //    //map transform
    //    bufferManager->MapTransformBuffer( renderableManager->m_renderableModel3D[ m_drawList[ x ].Index_ ].GetWorldMatrix( ) );

    //    //set model
    //    modelManager->BindModel( m_drawList[ x ].Model_ );

    //    //map texture
    //    textureManager->MapTextureByID( m_drawList[ x ].Material_ );

    //    shaderManager->Render( modelManager->GetModelVertcountByID( m_drawList[ x ].Model_ ) );
    //  }
    //  break;
    //  //renderable a point light
    //  case RENDERABLE_POINT_LIGHT:
    //  {
    //    break;
    //  }
    //  break;
    //  //rendering a directional light
    //  case RENDERABLE_DIRECTION_LIGHT:
    //  {
    //    break;
    //  }
    //  break;
    //  case RENDERABLE_PRIMITIVE:
    //  {
    //    if (!debugStart)
    //    {
    //      debugStart = true;
    //      bufferManager->MapCameraBuffer( currentCamera.GetViewMatrix( ), proj );
    //      dxCore->SetBlendState( BLEND_STATE_DEFAULT );
    //      dxCore->SetDepthState( DEPTH_STATE_DEPTH_CHECK );
    //      shaderManager->BindShader( SHADER_PRIMITIVE );
    //      layoutManager->SetInputLayout( SHADER_PRIMITIVE );
    //    }

    //    Primitive &prim = renderableManager->m_renderablePrimitives[ m_drawList[ x ].Index_ ];

    //    //set data if it is wireframe or not
    //    if (prim.GetWireFrameMode( ) == true)
    //    {
    //      dxCore->SetRasterState( RASTER_STATE_WIREFRAME_NO_CULL );
    //      dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP );
    //    }
    //    else
    //    {
    //      dxCore->SetRasterState( RASTER_STATE_BACKFACE_CULL );
    //      dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    //    }

    //    //set color
    //    PrimitiveColorBuffer pcb;
    //    pcb.color = prim.GetColor( );
    //    bufferManager->MapBuffer<BUFFER_PRIM_COLOR>( &pcb, PIXEL_SHADER );

    //    //render specific primitive, based upon data
    //    switch (prim.GetType( ))
    //    {
    //    case Primitive::PRIM_PLANE:
    //      dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );
    //      bufferManager->MapTransformBuffer( DirectX::XMMatrixScaling( prim.GetWidth( ), 0, prim.GetHeight( ) ) * prim.GetWorldMatrix( ) );
    //      modelManager->BindModel( modelManager->GetModelIDByName( "Plane" ) );
    //      shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "Plane" ) ) );
    //      break;

    //    case Primitive::PRIM_SPHERE:
    //      bufferManager->MapTransformBuffer( DirectX::XMMatrixScaling( prim.GetRadius( ) / 2.f, prim.GetRadius( ) / 2.f, prim.GetRadius( ) / 2.f ) * prim.GetWorldMatrix( ) );
    //      modelManager->BindModel( modelManager->GetModelIDByName( "Sphere" ) );
    //      shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "Sphere" ) ) );
    //      break;
    //    case Primitive::PRIM_CUBE:
    //      bufferManager->MapTransformBuffer( DirectX::XMMatrixScaling( prim.GetWidth( ), prim.GetHeight( ), prim.GetDepth( ) ) * prim.GetWorldMatrix( ) );
    //      modelManager->BindModel( modelManager->GetModelIDByName( "Cube" ) );
    //      shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "Cube" ) ) );
    //      break;
    //    case Primitive::PRIM_CAPSULE:
    //      modelManager->BindModel( modelManager->GetModelIDByName( "HalfSphere" ) );

    //      //render top cap
    //      bufferManager->MapTransformBuffer( DirectX::XMMatrixScaling( prim.GetRadius( ), prim.GetRadius( ), prim.GetRadius( ) )  * DirectX::XMMatrixTranslation( 0, prim.GetHeight( ) / 2.f, 0 ) * prim.GetWorldMatrix( ) );
    //      shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "HalfSphere" ) ) );
    //      //render bottom cap
    //      bufferManager->MapTransformBuffer( DirectX::XMMatrixScaling( prim.GetRadius( ), prim.GetRadius( ), prim.GetRadius( ) )  * DirectX::XMMatrixRotationX( 3.14f ) * DirectX::XMMatrixTranslation( 0, -prim.GetHeight( ) / 2.f, 0 ) * prim.GetWorldMatrix( ) );
    //      shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "HalfSphere" ) ) );

    //      //render body
    //      modelManager->BindModel( modelManager->GetModelIDByName( "CapsuleBody" ) );
    //      bufferManager->MapTransformBuffer( DirectX::XMMatrixScaling( prim.GetRadius( ), prim.GetHeight( ), prim.GetRadius( ) )  * prim.GetWorldMatrix( ) );
    //      shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "CapsuleBody" ) ) );
    //      break;
    //    }

    //  }
    //  break;
    //  default:
    //    UAssert( false, "Some rendering handle got messed up... Tell Matt" );
    //    break;
    //  }
    //}
    //gfxProfiler->Stamp( PROFILE_PRIMITIVES );

    ///////////////////////////////////////////////////////////////////
    //// RENDER POINTS AND LINES
    //shaderManager->BindShader( SHADER_PRIMITIVE );
    //layoutManager->SetInputLayout( SHADER_PRIMITIVE );
    //bufferManager->MapTransformBuffer( DirectX::XMMatrixIdentity( ) );
    //dxCore->SetRasterState( RASTER_STATE_BACKFACE_CULL );

    ////render points
    //if (drawingManager->CheckRenderPoints( ))
    //{
    //  ID3D11Buffer *mesh, *indices;
    //  unsigned vertCount, indexCount;
    //  drawingManager->ConstructPointMesh( vertCount, indexCount, &mesh, &indices );

    //  //set input
    //  dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_POINTLIST );

    //  //set up buffers
    //  bufferManager->MapCameraBuffer( currentCamera.GetViewMatrix( ), proj, GEOMETRY_SHADER );
    //  PointGeometryBuffer pgb;
    //  pgb.cameraUp = currentCamera.GetUp( );
    //  pgb.cameraPosition = currentCamera.GetPosition( );
    //  bufferManager->MapBuffer<BUFFER_POINT_GEOM>( &pgb, GEOMETRY_SHADER );

    //  //bind shader
    //  shaderManager->BindShader( SHADER_POINT );
    //  layoutManager->SetInputLayout( SHADER_POINT );

    //  //map meshes
    //  modelManager->BindMesh<PrimitiveVertex>( mesh, indices );

    //  //render
    //  shaderManager->Render( indexCount );

    //  //release resources
    //  RELEASE_RESOURCE( mesh );
    //  RELEASE_RESOURCE( indices );
    //}

    ////render lines
    //if (drawingManager->CheckRenderLines( ))
    //{
    //  ID3D11Buffer *mesh, *indices;
    //  unsigned vertCount, indexCount;
    //  drawingManager->ConstructLineMesh( vertCount, indexCount, &mesh, &indices );

    //  //set input
    //  dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP );

    //  //bind shader
    //  shaderManager->BindShader( SHADER_BASIC );
    //  layoutManager->SetInputLayout( SHADER_BASIC );

    //  //map meshes
    //  modelManager->BindMesh<PrimitiveVertex>( mesh, indices );

    //  //render
    //  shaderManager->Render( indexCount );

    //  //release resources
    //  RELEASE_RESOURCE( mesh );
    //  RELEASE_RESOURCE( indices );
    //}

    //gfxProfiler->Stamp( PROFILE_DEBUG );

    ////render main
    //dxCore->SetRasterState( RASTER_STATE_BACKFACE_CULL );
    //dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    //dxCore->SetDepthState( DEPTH_STATE_NO_DEPTH_CHECK );

    //modelManager->BindModel( modelManager->GetModelIDByName( "Quad" ) );
    //bufferManager->MapTransformBuffer( DirectX::XMMatrixScaling( -2 * width, 2 * height, 1 ) * DirectX::XMMatrixTranslation( posX, posY, 0 ) );
    //shaderManager->BindShader( SHADER_QUAD );
    //layoutManager->SetInputLayout( SHADER_QUAD );
    //dxCore->SetRenderTarget( RENDER_TARGET_SWAPCHAIN );

    ////RENDER SCENE
    //dxCore->SetBlendState( BLEND_STATE_DEFAULT );
    //bufferManager->MapCameraBuffer( DirectX::XMMatrixIdentity( ), DirectX::XMMatrixIdentity( ) );

    //dxCore->GetDeviceContext( )->PSSetShaderResources( 0, 1, &dxCore->GetRenderTargetMgr( )->GetRenderTarget( mainTarget )->ShaderMap );

    //shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "Quad" ) ) );

    ////clearing all buffers
    //textureManager->MapTextureByName( "Wire" );
    //textureManager->MapTextureByName( "Wire", 1 );
    //textureManager->MapTextureByName( "Wire", 2 );
    //textureManager->MapTextureByName( "Wire", 3 );

    //// end of temp //////////////////////////////////////////////////
  }

  void GraphicsCore::RenderScene_Deferred( float dt, GFXHND viewport )
  {
    /////////////////////////////////////////////////////////////////
    // PRE FRAME STUFF 
    // init buffers for frame
    dxCore->ClearDeferredBuffers( );
    gfxProfiler->Stamp( PROFILE_CLEAR_BUFFERS );

    // get viewport
    Viewport &vp = viewportManager->GetViewport( viewport );
    
    // get camera
    Camera &currentCamera = cameraManager->GetCamera( vp.GetViewportCamera( ) );

    //get d3d11 viewport info, set it
    D3D11_VIEWPORT vpData = vp.GetViewportData( );

    dxCore->GetDeviceContext( )->RSSetViewports( 1, &vpData );

    /////////////////////////////////////////////////////////////////
    // gets the projection matrix and view matrix
    DirectX::XMMATRIX proj, view;
    proj = currentCamera.GetProjMatrix( vpData.Width / 100, vpData.Height / 100 );
    view = currentCamera.GetViewMatrix( );

    /////////////////////////////////////////////////////////////////
    // SORT ALL DRAW CALLS
    std::sort( m_drawList.begin( ), m_drawList.begin( ) + m_drawCount, sort );

    /////////////////////////////////////////////////////////////////
    // BEGIN RENDERING
    //keep track of where we are
    int currentIndex = 0;

    //render 3d models deferred
    PrepFor3DModels( view, proj );
    while(m_drawList[ currentIndex ].Shader_ == SHADER_DEFERRED_DEPTH)
      Render3DModel( m_drawList[ currentIndex++ ] );

    //light pass
    PrepForLightPass( view, proj );
    while (m_drawList[ currentIndex ].Shader_ == SHADER_POINT_LIGHT)
      RenderPointLight( m_drawList[ currentIndex++ ], currentCamera );
    while (m_drawList[ currentIndex ].Shader_ == SHADER_DIRECTIONAL_LIGHT)
      RenderDirectionalLight( m_drawList[ currentIndex++ ], currentCamera );

    //primitive pass
    PrepForPrimitives( view, proj );
    while (m_drawList[ currentIndex ].Shader_ == SHADER_PRIMITIVE)
      RenderPrimitive( m_drawList[ currentIndex++ ] );

    //debug 
    PrepForDebugRender( );
    RenderDebugPoints( view, proj, currentCamera );
    RenderDebugLines( view, proj, currentCamera );

    /////////////////////////////////////////////////////////////////
    // RENDER MAIN //////////////////////////////////////////////////
    PrepForFinalOutput( );

    dxCore->GetDeviceContext( )->PSSetShaderResources( 0, 1, &dxCore->GetRenderTargetMgr( )->GetRenderTarget( RENDER_TARGET_DEFERRED_COLOR )->ShaderMap );
    dxCore->GetDeviceContext( )->PSSetShaderResources( 1, 1, &dxCore->GetRenderTargetMgr( )->GetRenderTarget( RENDER_TARGET_LIGHTMAP )->ShaderMap );
    shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "Quad" ) ) );
    gfxProfiler->Stamp( PROFILE_SCENE_MAIN );

    /////////////////////////////////////////////////////////////////
    //render primitive layer
    dxCore->GetDeviceContext( )->PSSetShaderResources( 0, 1, &dxCore->GetRenderTargetMgr( )->GetRenderTarget( RENDER_TARGET_DEBUG )->ShaderMap );

    shaderManager->BindShader( SHADER_QUAD );
    layoutManager->SetInputLayout( SHADER_QUAD );
    
    shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "Quad" ) ) );
    gfxProfiler->Stamp( PROFILE_SCENE_PRIMITIVE );

    //clearing all buffers
    textureManager->MapTextureByName( "Wire" );
    textureManager->MapTextureByName( "Wire", 1 );
    textureManager->MapTextureByName( "Wire", 2 );
    textureManager->MapTextureByName( "Wire", 3 );
  }

  void GraphicsCore::EndScene( )
  {
    //clear draw data
    memset( reinterpret_cast<unsigned long long*>(&m_drawList[ 0 ]), 0, sizeof( unsigned long long ) * m_drawCount );
    m_drawCount = 0;

    // render ui on top of everything
    if (m_renderUI)
    {
      PrepForUI( );
      shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "Quad" ) ) );
    }
    gfxProfiler->Stamp( PROFILE_SCENE_UI );

    // end profiler
    gfxProfiler->WaitForCalls( m_profile );

    // reset drawing for next frame
    drawingManager->EndScene( );

    // present
    dxCore->SwapChainBuffer( );

    // end the frame
    gfxProfiler->EndFrame( );

    //end rendering
    m_rendering = false;
  }

  // preparing for different stages /////////////////////////////////
  void GraphicsCore::PrepFor3DModels ( DirectX::XMMATRIX& view, DirectX::XMMATRIX& proj )
  {
    dxCore->SetBlendState( BLEND_STATE_DEFAULT );
    dxCore->SetDepthState( DEPTH_STATE_DEPTH_CHECK );

    //deferred shading
    dxCore->GetRenderTargetMgr( )->SetDeferredTargets( dxCore->GetDepthMgr( )->GetDepthStencilView( DEPTH_STENCIL_MAIN ) );
    shaderManager->BindShader( SHADER_DEFERRED_DEPTH );
    layoutManager->SetInputLayout( SHADER_DEFERRED_DEPTH );

    //set render type
    dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    //map the sampler
    textureManager->MapSamplerState( SAMPLER_WRAP_TEX );

    //set culling
    dxCore->SetRasterState( RASTER_STATE_NO_CULL );

    bufferManager->MapCameraBuffer( view, proj );
    bufferManager->MapCameraBuffer( view, proj, GEOMETRY_SHADER );
  }

  void GraphicsCore::PrepForLightPass ( DirectX::XMMATRIX& view, DirectX::XMMATRIX& proj )
  {
    gfxProfiler->Stamp( PROFILE_DEFERRED );
    dxCore->SetRenderTarget( RENDER_TARGET_LIGHTMAP );
    dxCore->SetBlendState( BLEND_STATE_ADDITIVE );
    dxCore->SetDepthState( DEPTH_STATE_NO_DEPTH_CHECK );

    bufferManager->MapCameraBuffer( view, proj );

    dxCore->GetDeviceContext( )->PSSetShaderResources( 0, 1, &dxCore->GetRenderTargetMgr( )->GetRenderTarget( RENDER_TARGET_DEFERRED_DEPTH )->ShaderMap );
    dxCore->GetDeviceContext( )->PSSetShaderResources( 1, 1, &dxCore->GetRenderTargetMgr( )->GetRenderTarget( RENDER_TARGET_DEFERRED_NORMAL )->ShaderMap );
    dxCore->GetDeviceContext( )->PSSetShaderResources( 2, 1, &dxCore->GetRenderTargetMgr( )->GetRenderTarget( RENDER_TARGET_DEFERRED_COLOR )->ShaderMap );
  }

  void GraphicsCore::PrepForPrimitives ( DirectX::XMMATRIX& view, DirectX::XMMATRIX& proj )
  {
    gfxProfiler->Stamp( PROFILE_LIGHTS );

    bufferManager->MapCameraBuffer( view, proj );
    dxCore->SetBlendState( BLEND_STATE_DEFAULT );
    dxCore->SetDepthState( DEPTH_STATE_DEPTH_CHECK );
    dxCore->SetRenderTarget( RENDER_TARGET_DEBUG );
    shaderManager->BindShader( SHADER_PRIMITIVE );
    layoutManager->SetInputLayout( SHADER_PRIMITIVE );
  }

  void GraphicsCore::PrepForDebugRender ( )
  {
    gfxProfiler->Stamp( PROFILE_PRIMITIVES );

    dxCore->SetBlendState( BLEND_STATE_NONE );
    dxCore->SetDepthState( DEPTH_STATE_DEPTH_CHECK );
    dxCore->SetRenderTarget( RENDER_TARGET_DEBUG );
    shaderManager->BindShader( SHADER_PRIMITIVE );
    layoutManager->SetInputLayout( SHADER_PRIMITIVE );
    bufferManager->MapTransformBuffer( DirectX::XMMatrixIdentity( ) );
    dxCore->SetRasterState( RASTER_STATE_LINE_RENDERING );
  }

  void GraphicsCore::PrepForFinalOutput ( )
  {
    gfxProfiler->Stamp( PROFILE_DEBUG );

    dxCore->SetRasterState( RASTER_STATE_BACKFACE_CULL );
    dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    dxCore->SetDepthState( DEPTH_STATE_NO_DEPTH_CHECK );
    dxCore->SetRenderTarget( RENDER_TARGET_SWAPCHAIN );
    dxCore->SetBlendState( BLEND_STATE_DEFAULT );

    modelManager->BindModel( modelManager->GetModelIDByName( "Quad" ) );
    shaderManager->BindShader( SHADER_DEFFERED_TEXTURE );
    layoutManager->SetInputLayout( SHADER_DEFFERED_TEXTURE );

    bufferManager->MapCameraBuffer( DirectX::XMMatrixIdentity( ), DirectX::XMMatrixIdentity( ) );
    bufferManager->MapTransformBuffer( DirectX::XMMatrixScaling( -2, 2, 1 ) );
  }

  void GraphicsCore::PrepForUI ( )
  {
    dxCore->SetBlendState( BLEND_STATE_DEFAULT );
    dxCore->SetRasterState( RASTER_STATE_UI );
    dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    dxCore->SetDepthState( DEPTH_STATE_NO_DEPTH_CHECK );
    dxCore->SetRenderTarget( RENDER_TARGET_SWAPCHAIN );
    bufferManager->MapCameraBuffer( DirectX::XMMatrixIdentity( ), DirectX::XMMatrixIdentity( ) );

    textureManager->MapSamplerState( SAMPLER_NO_FILTERING );
    shaderManager->BindShader( SHADER_UI );
    layoutManager->SetInputLayout( SHADER_UI );
    bufferManager->MapTransformBuffer( DirectX::XMMatrixScaling( -2, 2, 1 ) * DirectX::XMMatrixTranslation( 0, 0, 0.1f ) );
    modelManager->BindModel( modelManager->GetModelIDByName( "Quad" ) );

    dxCore->GetDeviceContext( )->PSSetShaderResources( 0, 1, &dxCore->GetRenderTargetMgr( )->GetRenderTarget( RENDER_TARGET_UI )->ShaderMap );
  }

  // rendering //////////////////////////////////////////////////////
  void GraphicsCore::Render3DModel ( DRAWHND handle )
  {
    POINT p;
    GetCursorPos( &p );

    static float temp = 0;
    temp += 0.016f;

    PrimitiveColorBuffer pcb;
    pcb.color.x = p.x / 100.f;
    pcb.color.y = p.y / 100.f;
    pcb.color.z = temp;
    bufferManager->MapBuffer<BUFFER_PRIM_COLOR>( &pcb, GEOMETRY_SHADER );

    //map transform
    bufferManager->MapTransformBuffer( renderableManager->m_renderableModel3D[ handle.Index_ ].GetWorldMatrix( ) );
    bufferManager->MapTransformBuffer( renderableManager->m_renderableModel3D[ handle.Index_ ].GetWorldMatrix( ), GEOMETRY_SHADER );

    //set model
    modelManager->BindModel( handle.Model_ );

    //map texture
    textureManager->MapTextureByID( handle.Material_ );

    shaderManager->Render( modelManager->GetModelVertcountByID( handle.Model_ ) );
  
  }

  void GraphicsCore::RenderPointLight ( DRAWHND handle, Camera &currentCamera )
  {
    shaderManager->BindShader( SHADER_POINT_LIGHT );
    layoutManager->SetInputLayout( SHADER_POINT_LIGHT );
    modelManager->BindModel( modelManager->GetModelIDByName( "Sphere" ) );

    PointLight &pl = renderableManager->m_renderablePointLight[ handle.Index_ ];

    //get camera position
    DirectX::XMFLOAT4 tempPos = currentCamera.GetPosition( );

    ////get light position
    DirectX::XMFLOAT3 lightP = pl.GetPosition( );

    //get light to camera vector
    DirectX::XMFLOAT3 camLight;
    camLight.x = tempPos.x - lightP.x;
    camLight.y = tempPos.y - lightP.y;
    camLight.z = tempPos.z - lightP.z;

    //set culling to backface or frontface, depending on the distance
    float radius = pl.GetRadius( ) / 2.f;
    float distance = camLight.x * camLight.x + camLight.y * camLight.y + camLight.z * camLight.z;
    float radiusSqr = radius * radius;

    //@Matt do not forget to look at this
    if (radiusSqr > fabs( distance ))
      dxCore->SetRasterState( RASTER_STATE_FRONTFACE_CULL );
    else
      dxCore->SetRasterState( RASTER_STATE_BACKFACE_CULL );

    //set buffer stuff
    bufferManager->MapTransformBuffer( DirectX::XMMatrixScaling( radius, radius, radius ) * DirectX::XMMatrixTranslation( lightP.x, lightP.y, lightP.z ) );
    DirectX::XMVECTOR lightPosition = DirectX::XMVector4Transform( DirectX::XMLoadFloat4( &DirectX::XMFLOAT4( pl.GetPosition( ).x, pl.GetPosition( ).y, pl.GetPosition( ).z, 1 ) ), currentCamera.GetViewMatrix( ) );
    PointLightBuffer pointB;
    pointB.lightPos = DirectX::XMFLOAT3( lightPosition.m128_f32 );
    pointB.lightRadius = radius;
    pointB.attenuation = 1;
    pointB.color.x = pl.GetColor( ).x;
    pointB.color.y = pl.GetColor( ).y;
    pointB.color.z = pl.GetColor( ).z;
    bufferManager->MapBuffer<BUFFER_POINT_LIGHT>( &pointB, PIXEL_SHADER );

    //render!
    shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "Sphere" ) ) );
  }

  void GraphicsCore::RenderDirectionalLight ( DRAWHND handle, Camera &currentCamera )
  {
    ID3D11ShaderResourceView *resource = dxCore->GetDepthMgr( )->GetDepthStencilSRV( DEPTH_STENCIL_SHADOWMAP );
    dxCore->GetDeviceContext( )->PSSetShaderResources( 3, 1, &resource );

    DirectionalLight &dl = renderableManager->m_renderableDirectionalLight[ handle.Index_ ];
    shaderManager->BindShader( SHADER_DIRECTIONAL_LIGHT );
    layoutManager->SetInputLayout( SHADER_DIRECTIONAL_LIGHT );
    dxCore->SetRasterState( RASTER_STATE_BACKFACE_CULL );

    bufferManager->MapTransformBuffer( DirectX::XMMatrixScaling( -2, 2, 1 ) );
    bufferManager->MapCameraBuffer( DirectX::XMMatrixIdentity( ), DirectX::XMMatrixIdentity( ) );
    modelManager->BindModel( modelManager->GetModelIDByName( "Quad" ) );

    DirectX::XMVECTOR lightDirection = DirectX::XMVector4Transform( DirectX::XMLoadFloat4( &DirectX::XMFLOAT4( dl.GetDirection( ).x, dl.GetDirection( ).y, dl.GetDirection( ).z, 0 ) ), currentCamera.GetViewMatrix( ) );
    DirectionalLightBuffer lightB;
    lightB.lightDirection = DirectX::XMFLOAT4( lightDirection.m128_f32 );
    lightB.lightColor = dl.GetColor( );
    lightB.attenuation = 1.f;
    lightB.lightPosition = DirectX::XMFLOAT4( lightDirection.m128_f32 );
    bufferManager->MapBuffer<BUFFER_DIRECTIONAL_LIGHT>( &lightB, PIXEL_SHADER );
    shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "Quad" ) ) );
  }

  void GraphicsCore::RenderPrimitive ( DRAWHND handle )
  {

    Primitive &prim = renderableManager->m_renderablePrimitives[ handle.Index_ ];

    //set data if it is wireframe or not
    if (prim.GetWireFrameMode( ) == true)
    {
      dxCore->SetRasterState( RASTER_STATE_WIREFRAME_NO_CULL );
      dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP );
    }
    else
    {
      dxCore->SetRasterState( RASTER_STATE_BACKFACE_CULL );
      dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    }

    //set color
    PrimitiveColorBuffer pcb;
    pcb.color = prim.GetColor( );
    bufferManager->MapBuffer<BUFFER_PRIM_COLOR>( &pcb, PIXEL_SHADER );

    //render specific primitive, based upon data
    switch (prim.GetType( ))
    {
    case Primitive::PRIM_PLANE:
      dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );
      bufferManager->MapTransformBuffer( DirectX::XMMatrixScaling( prim.GetWidth( ), 0, prim.GetHeight( ) ) * prim.GetWorldMatrix( ) );
      modelManager->BindModel( modelManager->GetModelIDByName( "Plane" ) );
      shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "Plane" ) ) );
      break;

    case Primitive::PRIM_SPHERE:
      bufferManager->MapTransformBuffer( DirectX::XMMatrixScaling( prim.GetRadius( ) / 2.f, prim.GetRadius( ) / 2.f, prim.GetRadius( ) / 2.f ) * prim.GetWorldMatrix( ) );
      modelManager->BindModel( modelManager->GetModelIDByName( "Sphere" ) );
      shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "Sphere" ) ) );
      break;
    case Primitive::PRIM_CUBE:
      bufferManager->MapTransformBuffer( DirectX::XMMatrixScaling( prim.GetWidth( ), prim.GetHeight( ), prim.GetDepth( ) ) * prim.GetWorldMatrix( ) );
      modelManager->BindModel( modelManager->GetModelIDByName( "Cube" ) );
      shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "Cube" ) ) );
      break;
    case Primitive::PRIM_CAPSULE:
      modelManager->BindModel( modelManager->GetModelIDByName( "HalfSphere" ) );

      //render top cap
      bufferManager->MapTransformBuffer( DirectX::XMMatrixScaling( prim.GetRadius( ), prim.GetRadius( ), prim.GetRadius( ) )  * DirectX::XMMatrixTranslation( 0, prim.GetHeight( ) / 2.f, 0 ) * prim.GetWorldMatrix( ) );
      shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "HalfSphere" ) ) );
      //render bottom cap
      bufferManager->MapTransformBuffer( DirectX::XMMatrixScaling( prim.GetRadius( ), prim.GetRadius( ), prim.GetRadius( ) )  * DirectX::XMMatrixRotationX( 3.14f ) * DirectX::XMMatrixTranslation( 0, -prim.GetHeight( ) / 2.f, 0 ) * prim.GetWorldMatrix( ) );
      shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "HalfSphere" ) ) );

      //render body
      modelManager->BindModel( modelManager->GetModelIDByName( "CapsuleBody" ) );
      bufferManager->MapTransformBuffer( DirectX::XMMatrixScaling( prim.GetRadius( ), prim.GetHeight( ), prim.GetRadius( ) )  * prim.GetWorldMatrix( ) );
      shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "CapsuleBody" ) ) );
      break;
    }
  }

  void GraphicsCore::RenderDebugPoints ( DirectX::XMMATRIX &view, DirectX::XMMATRIX &proj, Camera &currentCamera )
  {
    //render points
    if (drawingManager->CheckRenderPoints( ))
    {
      ID3D11Buffer *mesh, *indices;
      unsigned vertCount, indexCount;
      drawingManager->ConstructPointMesh( vertCount, indexCount, &mesh, &indices );

      //set input
      dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_POINTLIST );

      //set up buffers
      bufferManager->MapCameraBuffer( view, proj, GEOMETRY_SHADER );
      PointGeometryBuffer pgb;
      pgb.cameraUp = currentCamera.GetUp( );
      pgb.cameraPosition = currentCamera.GetPosition( );
      bufferManager->MapBuffer<BUFFER_POINT_GEOM>( &pgb, GEOMETRY_SHADER );

      //bind shader
      shaderManager->BindShader( SHADER_POINT );
      layoutManager->SetInputLayout( SHADER_POINT );

      //map meshes
      modelManager->BindMesh<PrimitiveVertex>( mesh, indices );

      //render
      shaderManager->Render( indexCount );
    }
  }

  void GraphicsCore::RenderDebugLines (DirectX::XMMATRIX &view, DirectX::XMMATRIX &proj, Camera &currentCamera)
  {
    //render lines
    if (drawingManager->CheckRenderLines( ))
    {
      ID3D11Buffer *mesh, *indices;
      unsigned vertCount, indexCount;
      drawingManager->ConstructLineMesh( vertCount, indexCount, &mesh, &indices );

      //set input
      dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );

      //bind shader
      shaderManager->BindShader( SHADER_BASIC );
      layoutManager->SetInputLayout( SHADER_BASIC );

      //map meshes
      modelManager->BindMesh<PrimitiveVertex>( mesh, indices );

      //render
      shaderManager->Render( indexCount );
    }
  }

  // misc stuff /////////////////////////////////////////////////////
  DXCore::DirectXCore *GraphicsCore::GetDXCore( )
  {
    return dxCore;
  }

  void GraphicsCore::Resize( int width, int height )
  {
    if (!m_ready)
      return;

    gfxInfo->SetDimensions( width, height );

    //what needs to be resized?
    //ui
    uiManager->Resize( width, height );

    return;

    //MAIN render targets, not viewports
    dxCore->ResizeDX( width, height );
  }
}