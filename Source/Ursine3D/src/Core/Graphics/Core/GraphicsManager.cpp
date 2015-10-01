#include "UrsinePrecompiled.h"
#include "GraphicsManager.h"

//@Matt
#include "VertexDefinitions.h"
#include <complex>
#include "DepthStencilStateList.h"

namespace ursine
{
    bool sort(_DRAWHND &h1, _DRAWHND &h2)
    {
        if (*reinterpret_cast<unsigned long long*>( &h1 ) < *reinterpret_cast<unsigned long long*>( &h2 ))
            return true;
        return false;
    }

    void GraphicsCore::Initialize(GfxConfig &config)
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
        m_currentlyRendering = false;
        m_sceneActive = false;

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
            shaderManager->LoadShader( SHADER_BILLBOARD2D, "BillboardedSprite" );
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

        Invalidate( );

        m_ready = true;
    }

    void GraphicsCore::Uninitialize()
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

    void GraphicsCore::Render(GFXHND handle)
    {
        UAssert( m_currentlyRendering == true, "Attempted to render an object without starting the frame!" );
        UAssert( m_sceneActive == true, "Attempted to render an object without beginning a scene!" );

        //convert to renderable handle
        _RENDERABLEHND *render = HND_RENDER( handle );

        //make sure right handle was passed
        UAssert( render->ID_ == ID_RENDERABLE, "Attempted to draw non-valid handle!" );

        //make sure we have enough room to render
        UAssert( m_drawCount < MAX_DRAW_CALLS, "Out of available draw calls! Let Matt know, easy fix." );

        //get a new draw call
        _DRAWHND &drawCall = m_drawList[ m_drawCount++ ];

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
            case RENDERABLE_BILLBOARD2D:
            {
                Billboard2D *current = &renderableManager->m_renderableBillboards[ render->Index_ ];

                drawCall.Index_ = render->Index_;
                drawCall.Type_ = render->Type_;
                drawCall.Material_ = textureManager->GetTextureIDByName( current->GetTextureName( ) );

                drawCall.Shader_ = SHADER_BILLBOARD2D;
            }
                break;
            default:
                break;
        }
    }

    void GraphicsCore::StartFrame()
    {
        UAssert( m_currentlyRendering == false, "Attempted to start the frame without ending the last one!" );
        m_currentlyRendering = true;

        while (m_rendering);

        m_rendering = true;


        dxCore->ClearSwapchain( );
        dxCore->ClearTargetBuffers( );
        dxCore->ClearDebugBuffer( );
        gfxProfiler->BeginFrame( );

        //cache state of all graphics objects
        renderableManager->CacheFrame( );
    }

    void GraphicsCore::BeginScene()
    {
        UAssert( m_currentlyRendering == true, "Attempted to begin a scene without starting the frame!" );
        UAssert( m_sceneActive == false, "Attempted to begin a scene without ending the last one!" );
        m_sceneActive = true;

        //clear draw call list
        memset( reinterpret_cast<unsigned long long*>( &m_drawList[ 0 ] ), 0, sizeof( unsigned long long) * m_drawCount );
        m_drawCount = 0;

        //clear debug buffer
        dxCore->ClearDebugBuffer( );
    }

    void GraphicsCore::RenderScene(float dt, GFXHND camera)
    {
        UAssert( m_currentlyRendering == true, "Attempted to render a scene without starting the frame!" );
        UAssert( m_sceneActive == true, "Attempted to render a scene without beginning one!" );

        // get viewport
        _RESOURCEHND *camHND = reinterpret_cast<_RESOURCEHND*>( &camera );
        UAssert( camHND->ID_ == ID_CAMERA, "Attempted to render UI with invalid camera!" );

        GFXCamera &cam = cameraManager->GetCamera( camera );

        //get game vp dimensions
        Viewport &gameVP = viewportManager->GetViewport( m_GameViewport );
        D3D11_VIEWPORT gvp = gameVP.GetViewportData( );

        //set directx viewport
        float w, h, x, y;
        cam.GetPosition( x, y );
        cam.GetDimensions( w, h );

        w *= gvp.Width;
        h *= gvp.Height;

        x = x * gvp.Width + gvp.TopLeftX;
        y = y * gvp.Height + gvp.TopLeftY;

        D3D11_VIEWPORT vpData;
        vpData.Width = w;
        vpData.Height = h;
        vpData.TopLeftX = x;
        vpData.TopLeftY = y;
        vpData.MinDepth = 0;
        vpData.MaxDepth = 1;

        dxCore->GetDeviceContext( )->RSSetViewports( 1, &vpData );

        //clear it
        dxCore->SetRasterState( RASTER_STATE_SOLID_BACKCULL );
        dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
        dxCore->SetDepthState( DEPTH_STATE_NODEPTH_NOSTENCIL );
        dxCore->SetRenderTarget( RENDER_TARGET_SWAPCHAIN );
        dxCore->SetBlendState( BLEND_STATE_DEFAULT );

        modelManager->BindModel( modelManager->GetModelIDByName( "internalQuad" ) );
        shaderManager->BindShader( SHADER_PRIMITIVE );
        layoutManager->SetInputLayout( SHADER_PRIMITIVE );

        bufferManager->MapCameraBuffer( SMat4::Identity( ), SMat4::Identity( ) );
        bufferManager->MapTransformBuffer( SMat4( -2, 2, 1 ) );

        PrimitiveColorBuffer pcb;
        //pcb.color = DirectX::XMFLOAT4( vp.GetBackgroundColor( ) );
        bufferManager->MapBuffer<BUFFER_PRIM_COLOR>( &pcb, PIXEL_SHADER );

        shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "internalQuad" ) ) );

        /////////////////////////////////////////////////////////////////
        if(cam.GetRenderMode() == VIEWPORT_RENDER_DEFERRED)
            RenderScene_Deferred( dt, camera );
        else
            RenderScene_Forward( dt, camera );

        return;
        //close thread handle if needed
        if (m_threadHandle != nullptr)
            CloseHandle( m_threadHandle );

        return;
        /*
        auto *data = new threadData;
        data->gfx = this;
        data->dt = dt;
        data->forward = viewportManager->GetRenderMode( viewport ) == VIEWPORT_RENDER_FORWARD;
        data->viewport = viewport;

        m_threadHandle = CreateThread( nullptr, 0, renderBootstrap, data, 0, &m_threadID );*/
    }

    DWORD GraphicsCore::renderBootstrap(LPVOID lpParam)
    {
        auto *data = reinterpret_cast<threadData*>( lpParam );

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

    void GraphicsCore::RenderScene_Deferred(float dt, GFXHND camera)
    {
        /////////////////////////////////////////////////////////////////
        // PRE FRAME STUFF 
        // init buffers for frame
        dxCore->ClearDeferredBuffers( );
        dxCore->ClearDepthBuffers( );
        gfxProfiler->Stamp( PROFILE_CLEAR_BUFFERS );

        // get camera
        GFXCamera &currentCamera = cameraManager->GetCamera( camera );

        //get d3d11 viewport info
        //get game vp dimensions
        Viewport &gameVP = viewportManager->GetViewport( m_GameViewport );
        D3D11_VIEWPORT gvp = gameVP.GetViewportData( );

        //set directx viewport
        float w, h;
        currentCamera.GetDimensions( w, h );

        w *= gvp.Width;
        h *= gvp.Height;

        /////////////////////////////////////////////////////////////////
        // gets the projection matrix and view matrix
        SMat4 proj, view;
        proj = currentCamera.GetProjMatrix( w, h );
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
        while (m_drawList[ currentIndex ].Shader_ == SHADER_DEFERRED_DEPTH)
            Render3DModel( m_drawList[ currentIndex++ ] );
        while (m_drawList[ currentIndex ].Shader_ == SHADER_BILLBOARD2D)
            Render2DBillboard( m_drawList[ currentIndex++ ] );

        //point light pass
        PrepForPointLightPass( view, proj );
        while (m_drawList[ currentIndex ].Shader_ == SHADER_POINT_LIGHT)
            RenderPointLight( m_drawList[ currentIndex++ ], currentCamera, proj );

        //directional light pass
        PrepForDirectionalLightPass( view, proj );
        while (m_drawList[ currentIndex ].Shader_ == SHADER_DIRECTIONAL_LIGHT)
            RenderDirectionalLight( m_drawList[ currentIndex++ ], currentCamera );

        //primitive pass
        PrepForPrimitives( view, proj );
        while (m_drawList[ currentIndex ].Shader_ == SHADER_PRIMITIVE)
            RenderPrimitive( m_drawList[ currentIndex++ ] );

        //debug 
        PrepForDebugRender( );
        dxCore->SetRasterState( RASTER_STATE_SOLID_BACKCULL );
        RenderDebugPoints( view, proj, currentCamera );
        dxCore->SetRasterState( RASTER_STATE_LINE_RENDERING );
        RenderDebugLines( view, proj, currentCamera );

        /////////////////////////////////////////////////////////////////
        // RENDER MAIN //////////////////////////////////////////////////
        PrepForFinalOutput( );

        dxCore->GetDeviceContext( )->PSSetShaderResources( 0, 1, &dxCore->GetRenderTargetMgr( )->GetRenderTarget( RENDER_TARGET_DEFERRED_COLOR )->ShaderMap );
        dxCore->GetDeviceContext( )->PSSetShaderResources( 1, 1, &dxCore->GetRenderTargetMgr( )->GetRenderTarget( RENDER_TARGET_LIGHTMAP )->ShaderMap );
        shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "internalQuad" ) ) );
        gfxProfiler->Stamp( PROFILE_SCENE_MAIN );

        /////////////////////////////////////////////////////////////////
        //render primitive layer
        dxCore->GetDeviceContext( )->PSSetShaderResources( 0, 1, &dxCore->GetRenderTargetMgr( )->GetRenderTarget( RENDER_TARGET_DEBUG )->ShaderMap );

        shaderManager->BindShader( SHADER_QUAD );
        layoutManager->SetInputLayout( SHADER_QUAD );

        shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "internalQuad" ) ) );
        gfxProfiler->Stamp( PROFILE_SCENE_PRIMITIVE );

        //clearing all buffers
        textureManager->MapTextureByName( "Wire" );
        textureManager->MapTextureByName( "Wire", 1 );
        textureManager->MapTextureByName( "Wire", 2 );
        textureManager->MapTextureByName( "Wire", 3 );
    }

    void GraphicsCore::RenderScene_Forward(float dt, GFXHND camera)
    {
        /////////////////////////////////////////////////////////////////
        // PRE FRAME STUFF 
        // init buffers for frame
        dxCore->ClearDeferredBuffers( );
        dxCore->ClearDepthBuffers( );
        gfxProfiler->Stamp( PROFILE_CLEAR_BUFFERS );

        // get camera
        GFXCamera &currentCamera = cameraManager->GetCamera( camera );

        //get d3d11 viewport info
        //get game vp dimensions
        Viewport &gameVP = viewportManager->GetViewport( m_GameViewport );
        D3D11_VIEWPORT gvp = gameVP.GetViewportData( );

        //set directx viewport
        float w, h;
        currentCamera.GetDimensions( w, h );

        w *= gvp.Width;
        h *= gvp.Height;

        /////////////////////////////////////////////////////////////////
        // gets the projection matrix and view matrix
        SMat4 proj, view;
        proj = currentCamera.GetProjMatrix( w, h );
        view = currentCamera.GetViewMatrix( );

        /////////////////////////////////////////////////////////////////
        // SORT ALL DRAW CALLS
        std::sort( m_drawList.begin( ), m_drawList.begin( ) + m_drawCount, sort );

        /////////////////////////////////////////////////////////////////
        // BEGIN RENDERING
        //keep track of where we are
        int currentIndex = 0;

        dxCore->SetBlendState( BLEND_STATE_DEFAULT );
        dxCore->SetDepthState( DEPTH_STATE_DEPTH_NOSTENCIL );
        dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
        dxCore->SetRasterState( RASTER_STATE_SOLID_BACKCULL );
        dxCore->SetRenderTarget( RENDER_TARGET_SWAPCHAIN );

        shaderManager->BindShader( SHADER_DIFFUSE );
        layoutManager->SetInputLayout( SHADER_DIFFUSE );
        textureManager->MapSamplerState( SAMPLER_WRAP_TEX );
        bufferManager->MapCameraBuffer( view, proj );

        //render objects
        while (m_drawList[ currentIndex ].Shader_ == SHADER_DEFERRED_DEPTH)
            Render3DModel( m_drawList[ currentIndex++ ] );
        while (m_drawList[ currentIndex ].Shader_ == SHADER_BILLBOARD2D)
            Render2DBillboard( m_drawList[ currentIndex++ ] );

        gfxProfiler->Stamp( PROFILE_LIGHTS );

        //go through lights
        while (m_drawList[ currentIndex ].Shader_ == SHADER_POINT_LIGHT)
            ++currentIndex;
        while (m_drawList[ currentIndex ].Shader_ == SHADER_DIRECTIONAL_LIGHT)
            ++currentIndex;

        //render primitives
        dxCore->SetRenderTarget( RENDER_TARGET_SWAPCHAIN );
        shaderManager->BindShader( SHADER_PRIMITIVE );
        layoutManager->SetInputLayout( SHADER_PRIMITIVE );

        while (m_drawList[ currentIndex ].Shader_ == SHADER_PRIMITIVE)
            RenderPrimitive( m_drawList[ currentIndex++ ] );

        //render points and lines
        gfxProfiler->Stamp( PROFILE_PRIMITIVES );
        dxCore->SetBlendState( BLEND_STATE_NONE );
        dxCore->SetRasterState( RASTER_STATE_LINE_RENDERING );
        dxCore->SetDepthState( DEPTH_STATE_DEPTH_NOSTENCIL );
        bufferManager->MapTransformBuffer( SMat4::Identity( ) );

        RenderDebugPoints( view, proj, currentCamera );

        dxCore->SetRasterState( RASTER_STATE_LINE_RENDERING );
        RenderDebugLines( view, proj, currentCamera );

        textureManager->MapTextureByName( "Wire" );
        textureManager->MapTextureByName( "Wire", 1 );
        textureManager->MapTextureByName( "Wire", 2 );
        textureManager->MapTextureByName( "Wire", 3 );
    }

    void GraphicsCore::EndScene()
    {
        UAssert( m_currentlyRendering == true, "Attemped to end a scene without starting the frame!" );
        UAssert( m_sceneActive == true, "Attempted to end a scene before beginning one!" );
        m_sceneActive = false;
    }

    void GraphicsCore::EndFrame()
    {
        UAssert( m_sceneActive == false, "Attempted to end the frame without ending the scene!" );
        UAssert( m_currentlyRendering == true, "Attemped to end the frame when it was never started!" );

        m_currentlyRendering = false;

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

        //check to resize
        dxCore->CheckSize( );

        //invalidate CPU-side gfx engine for next frame
        dxCore->Invalidate( );
        Invalidate( );
    }

    // preparing for different stages /////////////////////////////////
    void GraphicsCore::PrepFor3DModels(const SMat4 &view, const SMat4 &proj)
    {
        float blendFactor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };
        dxCore->GetDeviceContext( )->OMSetBlendState( nullptr, blendFactor, 0xffffffff );
        dxCore->SetDepthState( DEPTH_STATE_DEPTH_NOSTENCIL );

        //deferred shading
        dxCore->GetRenderTargetMgr( )->SetDeferredTargets( dxCore->GetDepthMgr( )->GetDepthStencilView( DEPTH_STENCIL_MAIN ) );
        shaderManager->BindShader( SHADER_DEFERRED_DEPTH );
        layoutManager->SetInputLayout( SHADER_DEFERRED_DEPTH );

        //set render type
        dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

        //map the sampler
        textureManager->MapSamplerState( SAMPLER_WRAP_TEX );

        //set culling
        dxCore->SetRasterState( RASTER_STATE_SOLID_BACKCULL );

        bufferManager->MapCameraBuffer( view, proj );

        //TEMP
        URSINE_TODO( "Remove this" );
        bufferManager->MapCameraBuffer( view, proj, GEOMETRY_SHADER );
    }

    void GraphicsCore::PrepForPointLightPass(const SMat4 &view, const SMat4 &proj)
    {
        gfxProfiler->Stamp( PROFILE_DEFERRED );

        modelManager->BindModel( modelManager->GetModelIDByName( "Sphere" ) );

        //set states
        dxCore->SetRenderTarget( RENDER_TARGET_LIGHTMAP, false );
        dxCore->SetBlendState( BLEND_STATE_ADDITIVE );
        dxCore->SetDepthState( DEPTH_STATE_NODEPTH_NOSTENCIL );
        dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

        //map inv proj
        InvProjBuffer ipb;
        SMat4 temp = proj;
        temp.Transpose( );
        temp.Inverse( );

        ipb.invProj = temp.ToD3D( );
        bufferManager->MapBuffer<BUFFER_INV_PROJ>( &ipb, PIXEL_SHADER );

        //map camera buffer
        bufferManager->MapCameraBuffer( view, proj );

        //map perspective values
        SMat4 tempMat = proj;
        tempMat.Transpose( );
        GBufferUnpackBuffer gbub;
        gbub.perspectiveVals.x = 1.f / tempMat( 0, 0 );
        gbub.perspectiveVals.y = 1.f / tempMat( 1, 1 );
        gbub.perspectiveVals.z = tempMat( 3, 2 );
        gbub.perspectiveVals.w = -tempMat( 2, 2 );
        bufferManager->MapBuffer<BUFFER_GBUFFER_UNPACK>( &gbub, PIXEL_SHADER );

        //bind shader
        shaderManager->BindShader( SHADER_POINT_LIGHT );
        layoutManager->SetInputLayout( SHADER_POINT_LIGHT );

        ////clear index/vertex buffers
        //dxCore->GetDeviceContext()->IASetVertexBuffers( 0, 0, nullptr, nullptr, nullptr );
        //dxCore->GetDeviceContext( )->IASetIndexBuffer( nullptr, DXGI_FORMAT_UNKNOWN, 0 );

        //set gbuffer resources
        ID3D11ShaderResourceView *srv = dxCore->GetDepthMgr( )->GetDepthStencilSRV( DEPTH_STENCIL_MAIN );
        dxCore->GetDeviceContext( )->PSSetShaderResources( 0, 1, &srv );
        dxCore->GetDeviceContext( )->PSSetShaderResources( 1, 1, &dxCore->GetRenderTargetMgr( )->GetRenderTarget( RENDER_TARGET_DEFERRED_COLOR )->ShaderMap );
        dxCore->GetDeviceContext( )->PSSetShaderResources( 2, 1, &dxCore->GetRenderTargetMgr( )->GetRenderTarget( RENDER_TARGET_DEFERRED_NORMAL )->ShaderMap );
        dxCore->GetDeviceContext( )->PSSetShaderResources( 3, 1, &dxCore->GetRenderTargetMgr( )->GetRenderTarget( RENDER_TARGET_DEFERRED_SPECPOW )->ShaderMap );
    }

    void GraphicsCore::PrepForDirectionalLightPass(const SMat4 &view, const SMat4 &proj)
    {
        modelManager->BindModel( modelManager->GetModelIDByName( "internalQuad" ) );

        shaderManager->BindShader( SHADER_DIRECTIONAL_LIGHT );
        layoutManager->SetInputLayout( SHADER_DIRECTIONAL_LIGHT );
        dxCore->SetRasterState( RASTER_STATE_SOLID_BACKCULL );

        bufferManager->MapCameraBuffer( SMat4::Identity( ), SMat4::Identity( ) );
        bufferManager->MapTransformBuffer( SMat4( -2, 2, 1 ) );
    }

    void GraphicsCore::PrepForPrimitives(const SMat4 &view, const SMat4 &proj)
    {
        gfxProfiler->Stamp( PROFILE_LIGHTS );

        bufferManager->MapCameraBuffer( view, proj );
        dxCore->SetBlendState( BLEND_STATE_DEFAULT );
        dxCore->SetDepthState( DEPTH_STATE_DEPTH_NOSTENCIL );
        dxCore->SetRenderTarget( RENDER_TARGET_DEBUG );
        shaderManager->BindShader( SHADER_PRIMITIVE );
        layoutManager->SetInputLayout( SHADER_PRIMITIVE );
    }

    void GraphicsCore::PrepForDebugRender()
    {
        gfxProfiler->Stamp( PROFILE_PRIMITIVES );

        dxCore->SetBlendState( BLEND_STATE_NONE );
        dxCore->SetDepthState( DEPTH_STATE_DEPTH_NOSTENCIL );
        dxCore->SetRenderTarget( RENDER_TARGET_DEBUG );
        shaderManager->BindShader( SHADER_PRIMITIVE );
        layoutManager->SetInputLayout( SHADER_PRIMITIVE );
        bufferManager->MapTransformBuffer( SMat4::Identity( ) );
        dxCore->SetRasterState( RASTER_STATE_LINE_RENDERING );
    }

    void GraphicsCore::PrepForFinalOutput()
    {
        gfxProfiler->Stamp( PROFILE_DEBUG );

        dxCore->SetRasterState( RASTER_STATE_SOLID_BACKCULL );
        dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
        dxCore->SetDepthState( DEPTH_STATE_NODEPTH_NOSTENCIL );
        dxCore->SetRenderTarget( RENDER_TARGET_SWAPCHAIN );
        dxCore->SetBlendState( BLEND_STATE_DEFAULT );

        modelManager->BindModel( modelManager->GetModelIDByName( "internalQuad" ) );
        shaderManager->BindShader( SHADER_DEFFERED_TEXTURE );
        layoutManager->SetInputLayout( SHADER_DEFFERED_TEXTURE );

        bufferManager->MapCameraBuffer( SMat4::Identity( ), SMat4::Identity( ) );
        bufferManager->MapTransformBuffer( SMat4( -2, 2, 1 ) );
    }

    void GraphicsCore::PrepForUI()
    {
        SMat4 trans;
        trans.Translate( SVec3( 0, 0, 0.1f ) );

        dxCore->SetBlendState( BLEND_STATE_DEFAULT );
        dxCore->SetRasterState( RASTER_STATE_UI );
        dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
        dxCore->SetDepthState( DEPTH_STATE_NODEPTH_NOSTENCIL );
        dxCore->SetRenderTarget( RENDER_TARGET_SWAPCHAIN );
        bufferManager->MapCameraBuffer( SMat4::Identity( ), SMat4::Identity( ) );

        textureManager->MapSamplerState( SAMPLER_NO_FILTERING );
        shaderManager->BindShader( SHADER_UI );
        layoutManager->SetInputLayout( SHADER_UI );
        bufferManager->MapTransformBuffer( SMat4( -2, 2, 1 ) * trans );
        modelManager->BindModel( modelManager->GetModelIDByName( "internalQuad" ) );
    }

    // rendering //////////////////////////////////////////////////////
    void GraphicsCore::Render3DModel(_DRAWHND handle)
    {
        //TEMP
        URSINE_TODO( "Remove this" );
        POINT point;
        GetCursorPos( &point );
        bufferManager->MapTransformBuffer( renderableManager->m_renderableModel3D[ handle.Index_ ].GetWorldMatrix( ), GEOMETRY_SHADER );
        PrimitiveColorBuffer pcb;
        pcb.color.x = point.x / 80.f;
        pcb.color.y = point.y / 80.f;

        static float t;
        t += 0.000016;
        pcb.color.z = t;
        bufferManager->MapBuffer<BUFFER_PRIM_COLOR>( &pcb, GEOMETRY_SHADER );
        //END OF TEMP

        //map transform
        bufferManager->MapTransformBuffer( renderableManager->m_renderableModel3D[ handle.Index_ ].GetWorldMatrix( ) );

        //map material data
        MaterialDataBuffer mdb;
        Model3D &current = renderableManager->m_renderableModel3D[ handle.Index_ ];
        current.GetMaterialData( mdb.emissive, mdb.specularPower, mdb.specularIntensity );
        bufferManager->MapBuffer<BUFFER_MATERIAL_DATA>( &mdb, PIXEL_SHADER );

        //set model
        modelManager->BindModel( handle.Model_ );

        //map texture
        textureManager->MapTextureByID( handle.Material_ );

        shaderManager->Render( modelManager->GetModelVertcountByID( handle.Model_ ) );
    }

    void GraphicsCore::Render2DBillboard(_DRAWHND handle)
    {
        //map transform
        bufferManager->MapTransformBuffer( renderableManager->m_renderableBillboards[ handle.Index_ ].GetWorldMatrix( ) );

        //set model
        modelManager->BindModel( "Sprite" );

        //map texture
        textureManager->MapTextureByID( handle.Material_ );

        //render
        shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "Sprite" ) ) );
    }

    void GraphicsCore::RenderPointLight(_DRAWHND handle, GFXCamera &currentCamera, SMat4 &proj)
    {
        //get point light data
        PointLight &pl = renderableManager->m_renderablePointLight[ handle.Index_ ];

        //get data
        float radius = pl.GetRadius( );

        //domain shader needs light proj
        SMat4 lightProj;
        lightProj = SMat4( radius, radius, radius ); //scaling
        lightProj *= SMat4( pl.GetPosition( ) ); //translate to world space
        lightProj *= currentCamera.GetViewMatrix( ); //transform into view space
        lightProj *= proj; //transform into screeen space

        //map
        bufferManager->MapBuffer<BUFFER_LIGHT_PROJ>( &lightProj, DOMAIN_SHADER );

        //ps needs point light data buffer
        SMat4 view = currentCamera.GetViewMatrix( ); //need to transpose view (dx11 gg)
        view.Transpose( );
        SVec3 lightPosition = view.TransformPoint( pl.GetPosition( ) );

        PointLightBuffer pointB;
        pointB.lightPos = lightPosition.ToD3D( );
        pointB.lightRadius = pl.GetRadius( );
        pointB.intensity = 1;
        pointB.color.x = pl.GetColor( ).r;
        pointB.color.y = pl.GetColor( ).g;
        pointB.color.z = pl.GetColor( ).b;
        bufferManager->MapBuffer<BUFFER_POINT_LIGHT>( &pointB, PIXEL_SHADER );

        //light transform
        SMat4 transform;
        transform *= SMat4( pl.GetPosition( ) );
        transform *= SMat4( radius, radius, radius );
        bufferManager->MapTransformBuffer( transform );

        //get camera position
        SVec3 tempPos = currentCamera.GetPosition( );

        ////get light position
        SVec3 lightP = pl.GetPosition( );
        SVec3 camLight = tempPos - lightP;
        float distance = camLight.LengthSquared( );
        float radiusSqr = radius * radius;

        if (radiusSqr > fabs( distance ))
            dxCore->SetRasterState( RASTER_STATE_SOLID_FRONTCULL );
        else
            dxCore->SetRasterState( RASTER_STATE_SOLID_BACKCULL );

        //render!
        shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "Sphere" ) ) );
    }

    void GraphicsCore::RenderDirectionalLight(_DRAWHND handle, GFXCamera &currentCamera)
    {
        //ID3D11ShaderResourceView *resource = dxCore->GetDepthMgr( )->GetDepthStencilSRV( DEPTH_STENCIL_SHADOWMAP );
        //dxCore->GetDeviceContext( )->PSSetShaderResources( 3, 1, &resource );

        DirectionalLight &dl = renderableManager->m_renderableDirectionalLight[ handle.Index_ ];

        SMat4 view = currentCamera.GetViewMatrix( ); //need to transpose view (dx11 gg)
        view.Transpose( );
        SVec3 lightDirection = view.TransformVector( dl.GetDirection( ) );

        DirectionalLightBuffer lightB;
        lightB.lightDirection.x = lightDirection.X( );
        lightB.lightDirection.y = lightDirection.Y( );
        lightB.lightDirection.z = lightDirection.Z( );

        lightB.intensity = 1.f;

        lightB.lightColor = DirectX::XMFLOAT3( dl.GetColor( ).r, dl.GetColor( ).g, dl.GetColor( ).b );

        bufferManager->MapBuffer<BUFFER_DIRECTIONAL_LIGHT>( &lightB, PIXEL_SHADER );
        shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "internalQuad" ) ) );
    }

    void GraphicsCore::RenderPrimitive(_DRAWHND handle)
    {
        Primitive &prim = renderableManager->m_renderablePrimitives[ handle.Index_ ];

        //set data if it is wireframe or not
        if (prim.GetWireFrameMode( ) == true)
        {
            dxCore->SetRasterState( RASTER_STATE_WIREFRAME_NOCULL );
            dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP );
        }
        else
        {
            dxCore->SetRasterState( RASTER_STATE_SOLID_BACKCULL );
            dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
        }

        //set color
        PrimitiveColorBuffer pcb;
        pcb.color = prim.GetColor( ).ToVector4( ).ToD3D( );
        bufferManager->MapBuffer<BUFFER_PRIM_COLOR>( &pcb, PIXEL_SHADER );

        //render specific primitive, based upon data
        switch (prim.GetType( ))
        {
            case Primitive::PRIM_PLANE:
                dxCore->GetDeviceContext( )->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );
                bufferManager->MapTransformBuffer( SMat4( prim.GetWidth( ), 0, prim.GetHeight( ) ) * prim.GetWorldMatrix( ) );
                modelManager->BindModel( modelManager->GetModelIDByName( "Plane" ) );
                shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "Plane" ) ) );
                break;

            case Primitive::PRIM_SPHERE:
                bufferManager->MapTransformBuffer( SMat4( prim.GetRadius( ) / 2.f, prim.GetRadius( ) / 2.f, prim.GetRadius( ) / 2.f ) * prim.GetWorldMatrix( ) );
                modelManager->BindModel( modelManager->GetModelIDByName( "Sphere" ) );
                shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "Sphere" ) ) );
                break;
            case Primitive::PRIM_CUBE:
                bufferManager->MapTransformBuffer( SMat4( prim.GetWidth( ), prim.GetHeight( ), prim.GetDepth( ) ) * prim.GetWorldMatrix( ) );
                modelManager->BindModel( modelManager->GetModelIDByName( "Cube" ) );
                shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "Cube" ) ) );
                break;
            case Primitive::PRIM_CAPSULE:
                modelManager->BindModel( modelManager->GetModelIDByName( "HalfSphere" ) );

                //render top cap
                SMat4 trans;
                trans.Translate( SVec3( 0.0f, prim.GetHeight( ) * 0.5f, 0.0f ) );
                bufferManager->MapTransformBuffer( SMat4( prim.GetRadius( ), prim.GetRadius( ), prim.GetRadius( ) ) * trans * prim.GetWorldMatrix( ) );
                shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "HalfSphere" ) ) );

                //render bottom cap
                SMat4 mat;
                mat.TRS( SVec3( 0, -prim.GetHeight( ) / 2.f, 0 ), SQuat( 180.0f, 0.0f, 0.0f ), SVec3( prim.GetRadius( ) ) );

                bufferManager->MapTransformBuffer( mat * prim.GetWorldMatrix( ) );
                shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "HalfSphere" ) ) );

                //render body
                modelManager->BindModel( modelManager->GetModelIDByName( "CapsuleBody" ) );
                bufferManager->MapTransformBuffer( SMat4( prim.GetRadius( ), prim.GetHeight( ), prim.GetRadius( ) ) * prim.GetWorldMatrix( ) );
                shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "CapsuleBody" ) ) );
                break;
        }
    }

    void GraphicsCore::RenderDebugPoints(const SMat4 &view, const SMat4 &proj, GFXCamera &currentCamera)
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
            pgb.cameraUp = SVec4( currentCamera.GetUp( ), 0 ).ToD3D( );
            pgb.cameraPosition = SVec4( currentCamera.GetPosition( ), 1 ).ToD3D( );
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

    void GraphicsCore::RenderDebugLines(const SMat4 &view, const SMat4 &proj, GFXCamera &currentCamera)
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

    void GraphicsCore::SetGameViewport(GFXHND vp)
    {
        _VIEWPORTHND *viewPort = reinterpret_cast<_VIEWPORTHND*>( &vp );

        UAssert( viewPort->ID_ == ID_VIEWPORT, "Attempted to set game viewport with invalid handle!" );

        m_GameViewport = vp;
    }

    // misc stuff /////////////////////////////////////////////////////
    DXCore::DirectXCore *GraphicsCore::GetDXCore()
    {
        return dxCore;
    }

    void GraphicsCore::Resize(int width, int height)
    {
        if (!m_ready)
            return;

        gfxInfo->SetDimensions( width, height );

        //what needs to be resized?
        //ui
        //@UI
        //uiManager->Resize( width, height );

        //MAIN render targets, not viewports
        dxCore->ResizeDX( width, height );

        Invalidate( );
    }

    void GraphicsCore::Invalidate()
    {
        dxCore->Invalidate( );
        shaderManager->Invalidate( );
        layoutManager->Invalidate( );
    }

    void GraphicsCore::RenderUI(GFXHND camera, RENDER_TARGETS input)
    {
        _RESOURCEHND *newRender = reinterpret_cast<_RESOURCEHND*>( &camera );

        UAssert( newRender->ID_ == ID_CAMERA, "Attempted to render UI with invalid camera!" );

        PrepForUI( );

        // get viewport
        GFXCamera &cam = cameraManager->GetCamera( camera );

        //get game vp dimensions
        Viewport &gameVP = viewportManager->GetViewport( m_GameViewport );
        D3D11_VIEWPORT gvp = gameVP.GetViewportData( );

        //set directx viewport
        float w, h, x, y;
        cam.GetPosition( x, y );
        cam.GetDimensions( w, h );

        w *= gvp.Width;
        h *= gvp.Height;

        x *= gvp.Width;
        y *= gvp.Height;

        D3D11_VIEWPORT vpData;
        vpData.Width = w;
        vpData.Height = h;
        vpData.TopLeftX = x;
        vpData.TopLeftY = y;
        vpData.MinDepth = 0;
        vpData.MaxDepth = 1;

        dxCore->GetDeviceContext( )->RSSetViewports( 1, &vpData );

        dxCore->GetDeviceContext( )->PSSetShaderResources( 0, 1, &dxCore->GetRenderTargetMgr( )->GetRenderTarget( input )->ShaderMap );
        shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "internalQuad" ) ) );
    }

    void GraphicsCore::RenderUI_Main(RENDER_TARGETS input)
    {
        PrepForUI( );

        //set directx viewport
        D3D11_VIEWPORT vpData = viewportManager->GetViewport( m_GameViewport ).GetViewportData( );
        unsigned w, h;
        gfxInfo->GetDimensions(w, h );
        vpData.TopLeftX = 0;
        vpData.TopLeftY = 0;
        vpData.Width = w;
        vpData.Height = h;

        dxCore->GetDeviceContext( )->RSSetViewports( 1, &vpData );

        dxCore->GetDeviceContext( )->PSSetShaderResources( 0, 1, &dxCore->GetRenderTargetMgr( )->GetRenderTarget( input )->ShaderMap );
        shaderManager->Render( modelManager->GetModelVertcountByID( modelManager->GetModelIDByName( "internalQuad" ) ) );
    }
}
