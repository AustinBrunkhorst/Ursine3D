/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GfxManager.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include <d3d11.h>
#include <complex>

#include "GfxManager.h"
#include "VertexDefinitions.h"
#include "DepthStencilStateList.h"
#include <Core/Graphics/Animation/Builder/AnimationState.h>
#include <Core/Graphics/Animation/Builder/AnimationBuilder.h>

// regular processors
#include "Billboard2DProcessor.h"
#include "DirectionalLightProcessor.h"
#include "Model3DProcessor.h"
#include "ParticleSystemProcessor.h"
#include "PointLightProcessor.h"
#include "SpotLightProcessor.h"
#include "SpriteTextProcessor.h"

#include "RenderPass.h"

// special passes
#include "LineRendererPass.h"
#include "PointRendererPass.h"

static int tempID = -1;
static HWND wHND = 0;

namespace ursine
{
    namespace graphics
    {
        bool sort(_DRAWHND &h1, _DRAWHND &h2)
        {
            if ( *reinterpret_cast<unsigned long long*>(&h1) < *reinterpret_cast<unsigned long long*>(&h2) )
                return true;
            return false;
        }

        void GfxManager::Initialize(GfxConfig &config)
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
            m_drawList.resize(MAX_DRAW_CALLS);
            m_drawCount = 0;
            m_profile = config.enableProfiling;
            m_threadHandle = nullptr;
            m_debug = config.enableDebugInfo;
            m_currentlyRendering = false;
            m_sceneActive = false;
            m_currentID = -1;

            RenderPass::SetGfxMgr(this);
            GraphicsEntityProcessor::SetGfxMgr(this);
            GlobalShaderResource::SetGfxMgr(this);

            //writing log stuff
            LogMessage("GRAPHICS");
            LogMessage("Graphics Config:", 1);
            LogMessage("Width: %i", 2, config.windowWidth);
            LogMessage("Height: %i", 2, config.windowHeight);
            LogMessage("Model Path: %s", 2, config.modelListPath.c_str());
            LogMessage("Texture Path: %s", 2, config.textureListPath.c_str());
            LogMessage("Shader Path: %s", 2, config.shaderListPath.c_str());
            LogMessage("Fullscreen: %s", 2, config.fullscreen == true ? "True" : "False");
            LogMessage("GPU Info", 1);
            gfxInfo->Initialize();
            gfxInfo->SetDimensions(config.windowWidth, config.windowHeight);

            wHND = config.handleToWindow;

            /////////////////////////////////////////////////////////////////
            // INITIALIZE MANAGERS //////////////////////////////////////////

            LogMessage("Initialize DirectX", 1);
            dxCore->Initialize(config.windowWidth, config.windowHeight, config.handleToWindow, gfxInfo, config.fullscreen, m_debug);

            LogMessage("Initialize Shaders", 1);
            shaderManager->Initialize(dxCore->GetDevice(), dxCore->GetDeviceContext(), config.shaderListPath);

            {
                //load shaders
                shaderManager->LoadShader(SHADER_BASIC, "BasicModelShader");
                shaderManager->LoadShader(SHADER_DIFFUSE, "DiffuseShader");
                shaderManager->LoadShader(SHADER_NORMAL, "NormalShader");
                shaderManager->LoadShader(SHADER_DEFFERED_TEXTURE, "DeferredTextureShader");
                shaderManager->LoadShader(SHADER_DEFERRED_DEPTH, "DeferredDepth");
                shaderManager->LoadShader(SHADER_DEFERRED_DEPTH_NORM, "DeferredDepthNormalMap");
                shaderManager->LoadShader(SHADER_DIRECTIONAL_LIGHT, "DirectionalLightSource");
                shaderManager->LoadShader(SHADER_SPOT_LIGHT, "SpotlightSource");
                shaderManager->LoadShader(SHADER_POINT_LIGHT, "PointLightSource");
                shaderManager->LoadShader(SHADER_QUAD, "QuadShader");
                shaderManager->LoadShader(SHADER_UI, "UIShader");
                shaderManager->LoadShader(SHADER_PRIMITIVE, "PrimitiveShader");
                shaderManager->LoadShader(SHADER_POINT, "PointShader");
                shaderManager->LoadShader(SHADER_SHADOW_PASS, "ShadowMapShader");
                shaderManager->LoadShader(SHADER_BILLBOARD2D, "BillboardedSprite");
                shaderManager->LoadShader(SHADER_PARTICLE, "ParticleShader");
                shaderManager->LoadShader(SHADER_EMISSIVE, "EmissiveShader");
                shaderManager->LoadShader(SHADER_FORWARD, "ForwardRenderer");
                shaderManager->LoadShader(SHADER_SPRITE_TEXT, "SpriteTextShader");
                
                //load compute
                shaderManager->LoadShader(SHADER_MOUSEPOSITION, "MouseTypeID");
            }

            LogMessage("Initialize Buffers", 1);
            bufferManager->Initialize(dxCore->GetDevice(), dxCore->GetDeviceContext());

            LogMessage("Initialize Models", 1);
            modelManager->Initialize(dxCore->GetDevice(), dxCore->GetDeviceContext(), config.modelListPath);

            renderableManager->Initialize();
            cameraManager->Initialize();

            LogMessage("Initialize Textures", 1);
            textureManager->Initialize(dxCore->GetDevice(), dxCore->GetDeviceContext(), config.textureListPath);

            viewportManager->Initialize(dxCore->GetRenderTargetMgr());

            uiManager->Initialize(dxCore->GetDevice(), dxCore->GetDeviceContext(), dxCore->GetRenderTargetMgr(), this);

            drawingManager->Initialize(dxCore->GetDevice(), dxCore->GetDeviceContext());

            gfxProfiler->Initialize(dxCore->GetDevice(), dxCore->GetDeviceContext(), m_profile);

            //create layouts
            layoutManager->Initialize(dxCore->GetDevice(), dxCore->GetDeviceContext(), shaderManager);

            //init drawing manager
            drawingManager->EndScene();

            Invalidate();

            m_ready = true;

            // load the font
            std::string textPath = "Assets/Bitmap Fonts/MainFont.fnt";
            m_font.Load(textPath);

            //textureManager->CreateTexture("Font", "Assets/Bitmap Fonts/"  + m_font.GetTextureFiles()[ 0 ], 512, 512);

            // TEST SHIT
            //RenderPass forwardPass("ForwardPass");

            //GlobalCBuffer<CameraBuffer, BUFFER_CAMERA> viewBuffer(SHADER_SLOT_0, SHADERTYPE_VERTEX);

            //GlobalGPUResource difuseTex(SHADER_SLOT_0, RESOURCE_INPUT_RT);
            //difuseTex.Update(RENDER_TARGET_DEFERRED_COLOR);
            //GlobalGPUResource specPow(SHADER_SLOT_1, RESOURCE_INPUT_RT);
            //specPow.Update(RENDER_TARGET_DEFERRED_SPECPOW);
            //GlobalGPUResource specInt(SHADER_SLOT_2, RESOURCE_INPUT_RT);
            //specInt.Update(RENDER_TARGET_DEFERRED_SPECPOW);

            //forwardPass.
            //    Set({ RENDER_TARGET_SWAPCHAIN }).
            //    Set(SHADER_BASIC).
            //    Set(DEPTH_STENCIL_MAIN).
            //    Set(DEPTH_STATE_DEPTH_NOSTENCIL).
            //    Set(SAMPLER_STATE_WRAP_TEX).
            //    Set(RASTER_STATE_SOLID_BACKCULL).
            //    Set(BLEND_STATE_DEFAULT).
            //    Set(DXCore::TOPOLOGY_TRIANGLE_LIST).
            //    Accepts(RENDERABLE_MODEL3D).
            //    AddResource(&difuseTex).
            //    AddResource(&specPow).
            //    AddResource(&specInt).
            //InitializePass();

        }

        void GfxManager::Uninitialize()
        {
            //WaitForSingleObject( m_threadHandle, INFINITE );

            gfxInfo->Uninitialize();
            shaderManager->Uninitialize();
            bufferManager->Uninitialize();
            layoutManager->Uninitialize();
            modelManager->Uninitialize();
            renderableManager->Uninitialize();
            cameraManager->Uninitialize();
            textureManager->Uninitialize();
            viewportManager->Uninitialize();
            uiManager->Uninitialize();
            drawingManager->Uninitialize();
            gfxProfiler->Uninitialize();

            //last
            dxCore->Uninitialize();

            delete gfxInfo;
            delete dxCore;
            delete shaderManager;
            delete bufferManager;
            delete layoutManager;
            delete modelManager;
            delete renderableManager;
            delete cameraManager;
            delete textureManager;
            delete viewportManager;
            delete uiManager;
            delete drawingManager;
            delete gfxProfiler;
        }

        void GfxManager::Render(GfxHND handle)
        {
            UAssert(m_currentlyRendering == true, "Attempted to render an object without starting the frame!");
            UAssert(m_sceneActive == true, "Attempted to render an object without beginning a scene!");

            //convert to renderable handle
            _RENDERABLEHND *render = HND_RENDER(handle);

            //make sure right handle was passed
            UAssert(render->ID_ == ID_RENDERABLE, "Attempted to draw non-valid handle!");

            //make sure we have enough room to render
            UAssert(m_drawCount < MAX_DRAW_CALLS, "Out of available draw calls! Let Matt know, easy fix.");

            //get a new draw call
            _DRAWHND &drawCall = m_drawList[ m_drawCount++ ];
            drawCall.buffer_ = 0;

            switch ( render->Type_ )
            {
                //rendering a model
            case RENDERABLE_MODEL3D:
            {
                Model3D *current = &renderableManager->m_renderableModel3D[ render->Index_ ];

                drawCall.Index_ = render->Index_;
                drawCall.Type_ = render->Type_;
                
                drawCall.Material_ = current->GetTextureHandle( ) & 0xFFFF;

                drawCall.Model_ = current->GetModelHandle( ) & 0xFFFF;
                drawCall.Overdraw_ = current->GetOverdraw();
                drawCall.Shader_ = drawCall.Overdraw_ ? SHADER_OVERDRAW_MODEL : SHADER_DEFERRED_DEPTH;
                drawCall.Type_ = drawCall.Overdraw_ ? RENDERABLE_OVERDRAW : render->Type_;
            }
            break;
            case RENDERABLE_BILLBOARD2D:
            {
                Billboard2D *current = &renderableManager->m_renderableBillboards[ render->Index_ ];

                drawCall.Index_ = render->Index_;
                drawCall.Type_ = render->Type_;

                GfxHND texHandle = current->GetTextureHandle( );
                drawCall.Material_ = texHandle & 0xFFFF;
                
                drawCall.Shader_ = SHADER_BILLBOARD2D;
                drawCall.Overdraw_ = current->GetOverdraw();
            }
            break;
            case RENDERABLE_LIGHT:
            {
                Light *current = &renderableManager->m_renderableLights[ render->Index_ ];

                drawCall.Index_ = render->Index_;
                drawCall.Type_ = render->Type_;
                drawCall.Overdraw_ = current->GetOverdraw();

                switch ( current->GetType() )
                {
                case Light::LIGHT_DIRECTIONAL:
                    drawCall.Shader_ = SHADER_DIRECTIONAL_LIGHT;
                    break;
                case Light::LIGHT_POINT:
                    drawCall.Shader_ = SHADER_POINT_LIGHT;
                    break;
                case Light::LIGHT_SPOTLIGHT:
                    drawCall.Shader_ = SHADER_SPOT_LIGHT;
                    break;
                default:
                    break;
                }
            }
            break;
            case RENDERABLE_PS:
            {
                ParticleSystem *current = &renderableManager->m_renderableParticleSystems[ render->Index_ ];

                GfxHND texHandle = current->GetTextureHandle( );
                drawCall.Material_ = texHandle & 0xFFFF;

                drawCall.Index_ = render->Index_;
                drawCall.Type_ = render->Type_;
                drawCall.Overdraw_ = current->GetOverdraw( );
                drawCall.Shader_ = SHADER_PARTICLE;
            }
            break;
            case RENDERABLE_SPRITE_TEXT:
            {
                SpriteText *current = &renderableManager->m_renderableSpriteText[ render->Index_ ];

                drawCall.Index_ = render->Index_;
                drawCall.Type_ = render->Type_;
                drawCall.Overdraw_ = current->GetOverdraw();
                drawCall.Shader_ = SHADER_SPRITE_TEXT;
            }
                break;
            default:
                break;
            }
        }

        void GfxManager::StartFrame()
        {
            UAssert(m_currentlyRendering == false, "Attempted to start the frame without ending the last one!");
            m_currentlyRendering = true;

            while ( m_rendering );

            m_rendering = true;

            dxCore->StartDebugEvent("FrameStart");

            dxCore->ClearSwapchain();
            dxCore->ClearTargetBuffers();
            dxCore->ClearDebugBuffer();
            gfxProfiler->BeginFrame();

            //cache state of all graphics objects
            renderableManager->CacheFrame();

            float colorArray[ 4 ] = { 0,0,0,1 };
            dxCore->GetDeviceContext()->ClearRenderTargetView(dxCore->GetRenderTargetMgr()->GetRenderTarget(RENDER_TARGET_DEFERRED_SPECPOW)->RenderTargetView, colorArray);
        }

        void GfxManager::BeginScene()
        {
            UAssert(m_currentlyRendering == true, "Attempted to begin a scene without starting the frame!");
            UAssert(m_sceneActive == false, "Attempted to begin a scene without ending the last one!");
            m_sceneActive = true;

            //clear draw call list
            memset(reinterpret_cast<unsigned long long*>(&m_drawList[ 0 ]), 0, sizeof(unsigned long long) * m_drawCount * 2);
            m_drawCount = 0;

            //clear debug buffer
            dxCore->ClearDebugBuffer();
        }

        void GfxManager::RenderScene(float dt, GfxHND camera)
        {
            UAssert(m_currentlyRendering == true, "Attempted to render a scene without starting the frame!");
            UAssert(m_sceneActive == true, "Attempted to render a scene without beginning one!");

            // get viewport
            _RESOURCEHND *camHND = reinterpret_cast<_RESOURCEHND*>(&camera);
            UAssert(camHND->ID_ == ID_CAMERA, "Attempted to render UI with invalid camera!");

            Camera &cam = cameraManager->GetCamera(camera);

            dxCore->StartDebugEvent("CameraRenderScene");

            //get game vp dimensions
            Viewport &gameVP = viewportManager->GetViewport(m_GameViewport);
            D3D11_VIEWPORT gvp = gameVP.GetViewportData();

            //set directx viewport
            float w, h, x, y;
            cam.GetViewportPosition(x, y);
            cam.GetDimensions(w, h);

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

            dxCore->GetDeviceContext()->RSSetViewports(1, &vpData);

            // clear it
            dxCore->SetRasterState(RASTER_STATE_SOLID_BACKCULL);
            dxCore->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            dxCore->SetDepthState(DEPTH_STATE_NODEPTH_NOSTENCIL);
            dxCore->SetRenderTarget(RENDER_TARGET_SWAPCHAIN, DEPTH_STENCIL_COUNT);
            dxCore->SetBlendState(BLEND_STATE_DEFAULT);

            modelManager->BindModel( INTERNAL_QUAD );
            shaderManager->BindShader(SHADER_PRIMITIVE);
            layoutManager->SetInputLayout(SHADER_PRIMITIVE);

            bufferManager->MapCameraBuffer(SMat4::Identity(), SMat4::Identity());
            bufferManager->MapTransformBuffer(SMat4(-2, 2, 1));

            PrimitiveColorBuffer pcb;
            //pcb.color = DirectX::XMFLOAT4( vp.GetBackgroundColor( ) );
            pcb.color = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
            bufferManager->MapBuffer<BUFFER_PRIM_COLOR>(&pcb, SHADERTYPE_PIXEL);

            shaderManager->Render( modelManager->GetModelIndexcountByID( INTERNAL_QUAD ) );


            /////////////////////////////////////////////////////////////////
            if ( cam.GetRenderMode() == VIEWPORT_RENDER_DEFERRED )
                RenderScene_Deferred(dt, camera);
            else
                RenderScene_Forward(dt, camera);

            dxCore->EndDebugEvent();
            return;
            //close thread handle if needed
            if ( m_threadHandle != nullptr )
                CloseHandle(m_threadHandle);

            return;
            /*
            auto *data = new threadData;
            data->gfx = this;
            data->dt = dt;
            data->forward = viewportManager->GetRenderMode( viewport ) == VIEWPORT_RENDER_FORWARD;
            data->viewport = viewport;

            m_threadHandle = CreateThread( nullptr, 0, renderBootstrap, data, 0, &m_threadID );*/
        }

        DWORD GfxManager::renderBootstrap(LPVOID lpParam)
        {
            auto *data = reinterpret_cast<threadData*>(lpParam);

            if ( data->forward == true )
            {
                data->gfx->RenderScene_Forward(data->dt, data->viewport);
            }
            else
            {
                data->gfx->RenderScene_Deferred(data->dt, data->viewport);
            }

            data->gfx->EndScene();

            delete lpParam;

            return 0;
        }

        void GfxManager::RenderScene_Deferred(float dt, GfxHND camera)
        {
            /////////////////////////////////////////////////////////////////
            // PRE FRAME STUFF 
            // init buffers for frame
            dxCore->ClearDeferredBuffers();
            dxCore->ClearDepthBuffers();
            dxCore->ClearDebugBuffer();

            // get camera
            Camera &currentCamera = cameraManager->GetCamera(camera);

            //get d3d11 viewport info
            //get game vp dimensions
            Viewport &gameVP = viewportManager->GetViewport(m_GameViewport);
            D3D11_VIEWPORT gvp = gameVP.GetViewportData();

            //set directx viewport
            float w, h;
            currentCamera.GetDimensions(w, h);

            w *= gvp.Width;
            h *= gvp.Height;

            currentCamera.SetScreenDimensions(w, h);
            currentCamera.SetScreenPosition(gvp.TopLeftX, gvp.TopLeftY);

            // sort the handles
            std::sort(m_drawList.begin(), m_drawList.begin() + m_drawCount, sort);

            // CREATE RENDER PIPELINE
            RenderPass DeferredPipeline;

            // define the passes
            RenderPass          deferredPass( "DeferredPass" );
            RenderPass          spotlightPass( "SpotlightPass" );
            RenderPass          pointlightPass( "PointLightPass" );
            RenderPass          directionalLightPass( "DirectionalLightPass" );
            RenderPass          emissivePass( "EmissivePass" );
            LineRendererPass    lineRenderPass( false );
            PointRendererPass   pointRenderPass( false );
            RenderPass          overdrawPass( "OverdrawPass" );
            LineRendererPass    overdrawLinePass( true, "OverdrawLine" );
            PointRendererPass   overdrawPointPass( true, "OverdrawPoint" );
            RenderPass          particlePass( "ParticlePass" );
            RenderPass          billboardPass( "BillboardPass" );
            RenderPass          textPass( "SpriteTextPass" );

            // create processors
            auto modelProcessor        = Model3DProcessor( );
            auto slProcessor           = SpotLightProcessor();
            auto plProcessor           = PointLightProcessor( );
            auto dlProcessor           = DirectionalLightProcessor( );
            auto particleProcessor     = ParticleSystemProcessor( );
            auto billboardPorcessor    = Billboard2DProcessor( );
            auto textProcessor         = SpriteTextProcessor( );
            
            // CREATE GLOBALS
            GlobalCBuffer<CameraBuffer, BUFFER_CAMERA>              viewBuffer( SHADERTYPE_VERTEX );
            GlobalCBuffer<CameraBuffer, BUFFER_CAMERA>              viewIdentity( SHADERTYPE_VERTEX );
            GlobalCBuffer<CameraBuffer, BUFFER_CAMERA>              viewBufferGeom( SHADERTYPE_GEOMETRY );
            GlobalCBuffer<PointGeometryBuffer, BUFFER_POINT_GEOM>   spriteGeomBuff( SHADERTYPE_GEOMETRY );
            GlobalCBuffer<TransformBuffer, BUFFER_TRANSFORM>        identityTransform( SHADERTYPE_VERTEX );
            GlobalCBuffer<TransformBuffer, BUFFER_TRANSFORM>        fullscreenTransform( SHADERTYPE_VERTEX );
            GlobalCBuffer<invViewBuffer, BUFFER_INV_PROJ>           invView( SHADERTYPE_VERTEX );
            GlobalCBuffer<invViewBuffer, BUFFER_INV_PROJ>           invProjection( SHADERTYPE_PIXEL );

            // input RTs
            GlobalGPUResource   depthInput( SHADER_SLOT_0, RESOURCE_INPUT_DEPTH );
            GlobalGPUResource   diffuseRT( SHADER_SLOT_1, RESOURCE_INPUT_RT );
            GlobalGPUResource   normalRT( SHADER_SLOT_2, RESOURCE_INPUT_RT );
            GlobalGPUResource   specPowRT( SHADER_SLOT_3, RESOURCE_INPUT_RT );
            GlobalGPUResource   debugInput( SHADER_SLOT_0, RESOURCE_INPUT_RT );
            GlobalGPUResource   lightmapRT( SHADER_SLOT_1, RESOURCE_INPUT_RT );

            // other resources
            GlobalGPUResource   spriteModel(SHADER_SLOT_0, RESOURCE_MODEL);
            GlobalGPUResource   lightConeModel(SHADER_SLOT_0, RESOURCE_MODEL);
            GlobalGPUResource   lightSphereModel(SHADER_SLOT_0, RESOURCE_MODEL);
            GlobalGPUResource   fullscreenModel(SHADER_SLOT_0, RESOURCE_MODEL);
            GlobalGPUResource   particleModel(SHADER_SLOT_0, RESOURCE_MODEL);
            GlobalGPUResource   fontTexture(SHADER_SLOT_0, RESOURCE_TEXTURE);
            
            /////////////////////////////////////////////////////////
            // DEFINING PIPELINE
            {
                /////////////////////////////////////////////////////////
                // DEFERRED PASS
                {
                    deferredPass.
                        Set( 
                            { 
                                RENDER_TARGET_DEFERRED_COLOR, 
                                RENDER_TARGET_DEFERRED_NORMAL, 
                                RENDER_TARGET_DEFERRED_SPECPOW 
                            } 
                        ).
                        Set( SHADER_DEFERRED_DEPTH ).
                        Set( DEPTH_STENCIL_MAIN ).
                        Set( DEPTH_STATE_DEPTH_NOSTENCIL ).
                        Set( SAMPLER_STATE_WRAP_TEX ).
                        Set( RASTER_STATE_SOLID_BACKCULL ).
                        Set( BLEND_STATE_COUNT ).
                        Set( DXCore::TOPOLOGY_TRIANGLE_LIST ).

                        AddResource( &viewBuffer ).

                        Accepts( RENDERABLE_MODEL3D ).
                        Processes( &modelProcessor ).
                    InitializePass( );
                }

                /////////////////////////////////////////////////////////
                // SPOTLIGHT PASS
                {
                    spotlightPass.
                        Set( { RENDER_TARGET_SWAPCHAIN } ).
                        Set( SHADER_SPOT_LIGHT ).
                        Set( DEPTH_STENCIL_COUNT ).
                        Set( DEPTH_STATE_NODEPTH_NOSTENCIL ).
                        Set( SAMPLER_STATE_WRAP_TEX ).
                        Set( RASTER_STATE_SOLID_BACKCULL ).
                        Set( BLEND_STATE_ADDITIVE ).
                        Set( DXCore::TOPOLOGY_TRIANGLE_LIST ).

                        AddResource( &viewBuffer ).
                        AddResource( &invProjection ).
                        AddResource( &lightConeModel ).

                        AddResource( &depthInput ).
                        AddResource( &diffuseRT ).
                        AddResource( &normalRT ).
                        AddResource( &specPowRT ).

                        Accepts( RENDERABLE_LIGHT ).
                        Processes( &slProcessor ).
                    InitializePass( );
                }

                /////////////////////////////////////////////////////////
                // POINT LIGHT PASS
                {
                    pointlightPass.
                        Set( { RENDER_TARGET_SWAPCHAIN } ).
                        Set( SHADER_POINT_LIGHT ).
                        Set( DEPTH_STENCIL_COUNT ).
                        Set( DEPTH_STATE_NODEPTH_NOSTENCIL ).
                        Set( SAMPLER_STATE_WRAP_TEX ).
                        Set( RASTER_STATE_SOLID_BACKCULL ).
                        Set( BLEND_STATE_ADDITIVE ).
                        Set( DXCore::TOPOLOGY_TRIANGLE_LIST ).

                        AddResource( &viewBuffer ).
                        AddResource( &invProjection ).
                        AddResource( &lightSphereModel ).

                        AddResource( &depthInput ).
                        AddResource( &diffuseRT ).
                        AddResource( &normalRT ).
                        AddResource( &specPowRT ).

                        Accepts( RENDERABLE_LIGHT ).
                        Processes( &plProcessor ).
                    InitializePass( );
                }

                /////////////////////////////////////////////////////////
                // DIRECTIONAL LIGHT PASS
                {
                    directionalLightPass.
                        Set( { RENDER_TARGET_SWAPCHAIN } ).
                        Set( SHADER_DIRECTIONAL_LIGHT ).
                        Set( DEPTH_STENCIL_MAIN ).
                        Set( DEPTH_STATE_NODEPTH_NOSTENCIL ).
                        Set( SAMPLER_STATE_WRAP_TEX ).
                        Set( RASTER_STATE_SOLID_BACKCULL ).
                        Set( BLEND_STATE_ADDITIVE ).
                        Set( DXCore::TOPOLOGY_TRIANGLE_LIST ).

                        AddResource( &viewIdentity ).
                        AddResource( &fullscreenTransform ).
                        AddResource( &fullscreenModel ).

                        AddResource( &depthInput ).
                        AddResource( &diffuseRT ).
                        AddResource( &normalRT ).
                        AddResource( &specPowRT ).

                        Accepts( RENDERABLE_LIGHT ).
                        Processes( &dlProcessor ).
                    InitializePass( );
                }

                /////////////////////////////////////////////////////////
                // EMISSIVE PASS
                {
                    emissivePass.
                        Set( { RENDER_TARGET_SWAPCHAIN } ).
                        Set( SHADER_EMISSIVE ).
                        Set( DEPTH_STENCIL_MAIN ).
                        Set( DEPTH_STATE_NODEPTH_NOSTENCIL ).
                        Set( SAMPLER_STATE_WRAP_TEX ).
                        Set( RASTER_STATE_SOLID_BACKCULL ).
                        Set( BLEND_STATE_ADDITIVE ).
                        Set( DXCore::TOPOLOGY_TRIANGLE_LIST ).

                        AddResource( &viewIdentity ).
                        AddResource( &fullscreenTransform ).
                        AddResource( &fullscreenModel ).

                        AddResource( &depthInput ).
                        AddResource( &diffuseRT ).
                        AddResource( &normalRT ).
                        AddResource( &specPowRT ).

                        IsFullscreenPass( true ).
                    InitializePass( );
                }

                /////////////////////////////////////////////////////////
                // LINE RENDER PASS
                {
                    lineRenderPass.
                        Set( { RENDER_TARGET_SWAPCHAIN } ).
                        Set( SHADER_BASIC ).
                        Set( DEPTH_STENCIL_MAIN ).
                        Set( DEPTH_STATE_DEPTH_NOSTENCIL ).
                        Set( SAMPLER_STATE_WRAP_TEX ).
                        Set( RASTER_STATE_LINE_RENDERING ).
                        Set( BLEND_STATE_COUNT ).
                        Set( DXCore::TOPOLOGY_LINE_LIST ).

                        AddResource( &identityTransform ).
                        AddResource( &viewBuffer ).

                        IsFullscreenPass( true ).
                    InitializePass( );
                }

                /////////////////////////////////////////////////////////
                // POINT RENDER PASS
                {
                    pointRenderPass.
                        Set( { RENDER_TARGET_SWAPCHAIN } ).
                        Set( SHADER_POINT ).
                        Set( DEPTH_STENCIL_MAIN ).
                        Set( DEPTH_STATE_DEPTH_NOSTENCIL ).
                        Set( SAMPLER_STATE_WRAP_TEX ).
                        Set( RASTER_STATE_SOLID_NOCULL ).
                        Set( BLEND_STATE_COUNT ).
                        Set( DXCore::TOPOLOGY_POINT_LIST ).

                        AddResource( &identityTransform ).
                        AddResource( &viewBuffer ).

                        IsFullscreenPass( true ).
                    InitializePass( );
                }

                /////////////////////////////////////////////////////////
                // OVERDRAW PASS
                {
                    overdrawPass.
                        Set(
                            {
                                RENDER_TARGET_DEFERRED_COLOR,
                                RENDER_TARGET_DEFERRED_NORMAL,
                                RENDER_TARGET_DEFERRED_SPECPOW
                            }
                        ).
                        Set( SHADER_DEFERRED_DEPTH ).
                        Set( DEPTH_STENCIL_OVERDRAW ).
                        Set( DEPTH_STATE_DEPTH_NOSTENCIL ).
                        Set( SAMPLER_STATE_WRAP_TEX ).
                        Set( RASTER_STATE_SOLID_NOCULL ).
                        Set( BLEND_STATE_COUNT ).
                        Set( DXCore::TOPOLOGY_TRIANGLE_LIST ).

                        AddResource( &viewBuffer ).

                        Accepts( RENDERABLE_OVERDRAW ).
                        Processes( &modelProcessor ).
                    InitializePass( );
                }

                /////////////////////////////////////////////////////////
                // OVERDRAW LINE PASS
                {
                    overdrawLinePass.
                        Set( { RENDER_TARGET_SWAPCHAIN } ).
                        Set( SHADER_BASIC ).
                        Set( DEPTH_STENCIL_COUNT ).
                        Set( DEPTH_STATE_NODEPTH_NOSTENCIL ).
                        Set( SAMPLER_STATE_WRAP_TEX ).
                        Set( RASTER_STATE_LINE_RENDERING ).
                        Set( BLEND_STATE_COUNT ).
                        Set( DXCore::TOPOLOGY_LINE_LIST ).

                        AddResource( &identityTransform ).
                        AddResource( &viewBuffer ).


                        IsFullscreenPass( true ).
                    InitializePass( );
                }

                /////////////////////////////////////////////////////////
                // OVERDRAW POINT PASS
                {
                    overdrawPointPass.
                        Set( { RENDER_TARGET_SWAPCHAIN } ).
                        Set( SHADER_POINT ).
                        Set( DEPTH_STENCIL_COUNT ).
                        Set( DEPTH_STATE_NODEPTH_NOSTENCIL ).
                        Set( SAMPLER_STATE_WRAP_TEX ).
                        Set( RASTER_STATE_SOLID_NOCULL ).
                        Set( BLEND_STATE_COUNT ).
                        Set( DXCore::TOPOLOGY_POINT_LIST ).

                        AddResource( &identityTransform ).
                        AddResource( &viewBuffer ).

                        IsFullscreenPass( true ).
                    InitializePass( );
                }

                /////////////////////////////////////////////////////////
                // PARTICLE PASS
                {
                    particlePass.
                        Set( { RENDER_TARGET_SWAPCHAIN } ).
                        Set( SHADER_PARTICLE ).
                        Set( DEPTH_STENCIL_MAIN ).
                        Set( DEPTH_STATE_CHECKDEPTH_NOWRITE_NOSTENCIL ).
                        Set( SAMPLER_STATE_WRAP_TEX ).
                        Set( RASTER_STATE_SOLID_NOCULL ).
                        Set( BLEND_STATE_ADDITIVE ).
                        Set( DXCore::TOPOLOGY_TRIANGLE_LIST ).

                        AddResource( &viewBuffer ).
                        AddResource( &particleModel ).
                        AddResource( &invView ).

                        Accepts( RENDERABLE_PS ).
                        Processes( &particleProcessor ).
                        OverrideLayout( SHADER_OVERRIDE ).
                    InitializePass( );
                }
            
                /////////////////////////////////////////////////////////
                // BILLBOARD PASS
                {
                    billboardPass.
                        Set(
                            {
                                RENDER_TARGET_SWAPCHAIN,
                                RENDER_TARGET_DEFERRED_NORMAL,
                                RENDER_TARGET_DEFERRED_SPECPOW
                            }
                        ).
                        Set( SHADER_BILLBOARD2D ).
                        Set( DEPTH_STENCIL_MAIN ).
                        Set( DEPTH_STATE_DEPTH_NOSTENCIL ).
                        Set( SAMPLER_STATE_WRAP_TEX ).
                        Set( RASTER_STATE_SOLID_BACKCULL ).
                        Set( BLEND_STATE_COUNT ).
                        Set( DXCore::TOPOLOGY_POINT_LIST ).

                        AddResource( &viewBuffer ).
                        AddResource( &spriteModel ).
                        AddResource( &viewBufferGeom ).
                        AddResource( &spriteGeomBuff).

                        Accepts( RENDERABLE_BILLBOARD2D ).
                        Processes( &billboardPorcessor ).
                    InitializePass( );
                }
            
                /////////////////////////////////////////////////////////
                // SPRITE TEXT PASS
                {
                    textPass.
                         Set(
                            { 
                                RENDER_TARGET_SWAPCHAIN,
                                RENDER_TARGET_DEFERRED_NORMAL,
                                RENDER_TARGET_DEFERRED_SPECPOW
                            }
                        ).
                        Set( SHADER_SPRITE_TEXT ).
                        Set( DEPTH_STENCIL_MAIN ).
                        Set( DEPTH_STATE_DEPTH_NOSTENCIL ).
                        Set( SAMPLER_STATE_WRAP_TEX ).
                        Set( RASTER_STATE_SOLID_NOCULL ).
                        Set( BLEND_STATE_COUNT ).
                        Set( DXCore::TOPOLOGY_TRIANGLE_LIST ).

                        AddResource( &viewBuffer ).
                        AddResource( &particleModel ).
                        AddResource( &invView ).
                        AddResource( &fontTexture ).

                        Accepts( RENDERABLE_SPRITE_TEXT ).
                        Processes( &textProcessor ).
                        OverrideLayout(SHADER_OVERRIDE).
                    InitializePass( );
                }
            }

            /////////////////////////////////////////////////////////
            // CREATE PIPELINE
            DeferredPipeline.
                AddPrePass( &deferredPass ).
                AddPrePass( &spotlightPass ).
                AddPrePass( &pointlightPass ).
                AddPrePass( &directionalLightPass ).
                AddPrePass( &emissivePass ).
                AddPrePass( &lineRenderPass ).
                AddPrePass( &pointRenderPass ).
                AddPrePass( &overdrawPass ).
                AddPrePass( &overdrawLinePass ).
                AddPrePass( &overdrawPointPass ).
                AddPrePass( &particlePass ).
                AddPrePass( &billboardPass ).
                AddPrePass( &textPass ).
            InitializePass( );

            /////////////////////////////////////////////////////////
            // UPDATE RESOURCES
            // CONSTANT BUFFERS//////////////////
            CameraBuffer cb;
            PointGeometryBuffer pgb;
            TransformBuffer tb;
            invViewBuffer ivb;

            // viewBuffer(SHADERTYPE_VERTEX);
            // viewBufferGeom(SHADERTYPE_GEOMETRY);
            cb.view = SMat4::Transpose( currentCamera.GetViewMatrix( ) ).ToD3D( );
            cb.projection = SMat4::Transpose( currentCamera.GetProjMatrix( ) ).ToD3D( );
            viewBuffer.Update( cb );
            viewBufferGeom.Update( cb );

            // viewIdentity(SHADERTYPE_VERTEX);
            cb.view = SMat4::Identity( ).ToD3D( );
            cb.projection = SMat4::Identity( ).ToD3D( );
            viewIdentity.Update( cb );
            
            // spriteGeomBuff(SHADERTYPE_GEOMETRY);
            pgb.cameraPosition = SVec4(currentCamera.GetPosition(), 1).ToD3D();
            pgb.cameraUp = SVec4(currentCamera.GetUp(), 0).ToD3D();
            spriteGeomBuff.Update(pgb);
            
            // identityTransform(SHADERTYPE_VERTEX);
            tb.transform = SMat4::Identity( ).ToD3D( );
            identityTransform.Update( tb );

            // fullscreenTransform(SHADERTYPE_VERTEX);
            tb.transform = SMat4(-2, 2, 1).ToD3D( );
            fullscreenTransform.Update( tb );

            // invView( SHADERTYPE_VERTEX );
            SMat4 temp = currentCamera.GetViewMatrix( );
            temp.Transpose( );
            temp.Inverse( );
            ivb.invView = temp.ToD3D( );
            invView.Update( ivb, SHADER_SLOT_4 );

            // invProjection( SHADERTYPE_PIXEL );
            temp = currentCamera.GetProjMatrix( );
            temp.Transpose( );
            temp.Inverse( );
            ivb.invView = temp.ToD3D( );
            currentCamera.GetPlanes( ivb.nearPlane, ivb.farPlane );
            invProjection.Update( ivb );

            // TARGET INPUTS //////////////////
            // input RTs
            // depthInput(SHADER_SLOT_0, RESOURCE_INPUT_DEPTH);
            depthInput.Update( DEPTH_STENCIL_MAIN );

            // diffuseRT(SHADER_SLOT_1, RESOURCE_INPUT_RT);
            diffuseRT.Update( RENDER_TARGET_DEFERRED_COLOR );

            // normalRT(SHADER_SLOT_2, RESOURCE_INPUT_RT);
            normalRT.Update( RENDER_TARGET_DEFERRED_NORMAL );

            // specPowRT(SHADER_SLOT_3, RESOURCE_INPUT_RT);
            specPowRT.Update( RENDER_TARGET_DEFERRED_SPECPOW );
            // debugInput(SHADER_SLOT_0, RESOURCE_INPUT_RT);
            debugInput.Update( RENDER_TARGET_DEBUG );

            // lightmapRT(SHADER_SLOT_1, RESOURCE_INPUT_RT);
            lightmapRT.Update( RENDER_TARGET_LIGHTMAP );

            // TEXTURES AND MODELS /////////////
            lightConeModel.Update( 0 );
            lightSphereModel.Update( 0 );
            fullscreenModel.Update( INTERNAL_QUAD );
            spriteModel.Update( INTERNAL_QUAD );
            particleModel.Update( INTERNAL_POINT_INDICES );
            fontTexture.Update( 0 );

            /////////////////////////////////////////////////////////
            // RENDER
            DeferredPipeline.Execute( currentCamera );

            return;
        }

        void GfxManager::RenderScene_Forward(float dt, GfxHND camera)
        {
            ///////////////////////////////////////////////////////////////////
            //// PRE FRAME STUFF 
            //// init buffers for frame
            dxCore->ClearDeferredBuffers();
            dxCore->ClearDepthBuffers();
            dxCore->ClearDebugBuffer();

            // get camera
            Camera &currentCamera = cameraManager->GetCamera(camera);

            //get d3d11 viewport info
            //get game vp dimensions
            Viewport &gameVP = viewportManager->GetViewport(m_GameViewport);
            D3D11_VIEWPORT gvp = gameVP.GetViewportData();

            //set directx viewport
            float w, h;
            currentCamera.GetDimensions(w, h);

            w *= gvp.Width;
            h *= gvp.Height;

            currentCamera.SetScreenDimensions(w, h);
            currentCamera.SetScreenPosition(gvp.TopLeftX, gvp.TopLeftY);

            // sort the handles
            std::sort(m_drawList.begin(), m_drawList.begin() + m_drawCount, sort);

            // CREATE RENDER PIPELINE
            RenderPass MainPipeline;

            // define the passes
            RenderPass forwardPass( "ForwardPass" );
            RenderPass billboardPass( "BillboardPass" );
            LineRendererPass lineRenderPass( false );
            LineRendererPass overdrawLinePass( true, "OverdrawLine" );
            RenderPass particlePass( "ParticlePass" );
            RenderPass overdrawPass( "OverdrawPass" );
            RenderPass textPass( "SpriteTextPass" );
            PointRendererPass pointRenderPass( false );
            PointRendererPass overdrawPointPass( true, "OverdrawPoint" );

            // create processors
            auto modelProcessor     = Model3DProcessor( );
            auto billboardPorcessor = Billboard2DProcessor( );
            auto particleProcessor  = ParticleSystemProcessor( );
            auto textProcessor      = SpriteTextProcessor( );

            // CREATE GLOBALS
            GlobalCBuffer<CameraBuffer, BUFFER_CAMERA>                      viewBuffer(SHADERTYPE_VERTEX);
            GlobalCBuffer<CameraBuffer, BUFFER_CAMERA>                      viewBufferGeom(SHADERTYPE_GEOMETRY);
            GlobalCBuffer<PointGeometryBuffer, BUFFER_POINT_GEOM>           geomBuff(SHADERTYPE_GEOMETRY);
            GlobalCBuffer<TransformBuffer, BUFFER_TRANSFORM>                identity(SHADERTYPE_VERTEX);
            GlobalCBuffer<invViewBuffer, BUFFER_INV_PROJ>                   invView(SHADERTYPE_VERTEX);
            
            GlobalGPUResource spriteModel(SHADER_SLOT_0, RESOURCE_MODEL);
            GlobalGPUResource particleModel(SHADER_SLOT_0, RESOURCE_MODEL);
            GlobalGPUResource fontTexture(SHADER_SLOT_0, RESOURCE_TEXTURE);

            {
                /////////////////////////////////////////////////////////
                // FORWARD PASS
                forwardPass.
                    Set( 
                        { 
                            RENDER_TARGET_SWAPCHAIN, 
                            RENDER_TARGET_DEFERRED_NORMAL, 
                            RENDER_TARGET_DEFERRED_SPECPOW 
                        } 
                    ).
                    Set( SHADER_FORWARD ).
                    Set( DEPTH_STENCIL_MAIN ).
                    Set( DEPTH_STATE_DEPTH_NOSTENCIL ).
                    Set( SAMPLER_STATE_WRAP_TEX ).
                    Set( RASTER_STATE_SOLID_BACKCULL ).
                    Set( BLEND_STATE_COUNT ).
                    Set( DXCore::TOPOLOGY_TRIANGLE_LIST ).

                    AddResource( &viewBuffer ).

                    Accepts( RENDERABLE_MODEL3D ).
                    Processes( &modelProcessor ).
                InitializePass( );

                /////////////////////////////////////////////////////////
                // BILLBOARD PASS
                billboardPass.
                    Set(
                        {
                            RENDER_TARGET_SWAPCHAIN,
                            RENDER_TARGET_DEFERRED_NORMAL,
                            RENDER_TARGET_DEFERRED_SPECPOW
                        }
                    ).
                    Set( SHADER_BILLBOARD2D ).
                    Set( DEPTH_STENCIL_MAIN ).
                    Set( DEPTH_STATE_DEPTH_NOSTENCIL ).
                    Set( SAMPLER_STATE_WRAP_TEX ).
                    Set( RASTER_STATE_SOLID_BACKCULL ).
                    Set( BLEND_STATE_COUNT ).
                    Set( DXCore::TOPOLOGY_POINT_LIST ).

                    AddResource( &viewBuffer ).
                    AddResource( &spriteModel ).
                    AddResource( &viewBufferGeom ).
                    AddResource( &geomBuff ).

                    Accepts( RENDERABLE_BILLBOARD2D ).
                    Processes( &billboardPorcessor ).
                InitializePass( );

                /////////////////////////////////////////////////////////
                // LINE RENDER PASS
                lineRenderPass.
                    Set( { RENDER_TARGET_SWAPCHAIN } ).
                    Set( SHADER_BASIC ).
                    Set( DEPTH_STENCIL_MAIN ).
                    Set( DEPTH_STATE_DEPTH_NOSTENCIL ).
                    Set( SAMPLER_STATE_WRAP_TEX ).
                    Set( RASTER_STATE_LINE_RENDERING ).
                    Set( BLEND_STATE_COUNT ).
                    Set( DXCore::TOPOLOGY_LINE_LIST ).

                    AddResource(&identity).
                InitializePass( );

                overdrawLinePass.
                    Set( { RENDER_TARGET_SWAPCHAIN } ).
                    Set( SHADER_BASIC ).
                    Set( DEPTH_STENCIL_COUNT ).
                    Set( DEPTH_STATE_NODEPTH_NOSTENCIL ).
                    Set( SAMPLER_STATE_WRAP_TEX ).
                    Set( RASTER_STATE_LINE_RENDERING ).
                    Set( BLEND_STATE_COUNT ).
                    Set( DXCore::TOPOLOGY_LINE_LIST ).

                    AddResource(&identity).
                InitializePass();

                /////////////////////////////////////////////////////////
                // POINT RENDER PASS
                pointRenderPass.
                    Set( { RENDER_TARGET_SWAPCHAIN } ).
                    Set( SHADER_POINT ).
                    Set( DEPTH_STENCIL_MAIN ).
                    Set( DEPTH_STATE_DEPTH_NOSTENCIL ).
                    Set( SAMPLER_STATE_WRAP_TEX ).
                    Set( RASTER_STATE_SOLID_NOCULL ).
                    Set( BLEND_STATE_COUNT ).
                    Set( DXCore::TOPOLOGY_POINT_LIST ).

                    AddResource( &identity ).
                InitializePass( );

                overdrawPointPass.
                    Set( { RENDER_TARGET_SWAPCHAIN } ).
                    Set( SHADER_POINT ).
                    Set( DEPTH_STENCIL_COUNT ).
                    Set( DEPTH_STATE_NODEPTH_NOSTENCIL ).
                    Set( SAMPLER_STATE_WRAP_TEX ).
                    Set( RASTER_STATE_SOLID_NOCULL ).
                    Set( BLEND_STATE_COUNT ).
                    Set( DXCore::TOPOLOGY_POINT_LIST ).

                    AddResource( &identity ).
                InitializePass( );

                /////////////////////////////////////////////////////////
                // PARTICLE PASS
                particlePass.
                    Set( { RENDER_TARGET_SWAPCHAIN } ).
                    Set( SHADER_PARTICLE ).
                    Set( DEPTH_STENCIL_MAIN ).
                    Set( DEPTH_STATE_CHECKDEPTH_NOWRITE_NOSTENCIL ).
                    Set( SAMPLER_STATE_WRAP_TEX ).
                    Set( RASTER_STATE_SOLID_NOCULL ).
                    Set( BLEND_STATE_ADDITIVE ).
                    Set( DXCore::TOPOLOGY_TRIANGLE_LIST ).

                    AddResource( &viewBuffer ).
                    AddResource( &particleModel ).
                    AddResource( &invView ).

                    Accepts( RENDERABLE_PS ).
                    Processes( &particleProcessor ).
                    OverrideLayout( SHADER_OVERRIDE ).
                InitializePass( );

                /////////////////////////////////////////////////////////
                // OVERDRAW PASS
                overdrawPass.
                      Set( 
                        { 
                            RENDER_TARGET_DEFERRED_COLOR, 
                            RENDER_TARGET_DEFERRED_NORMAL, 
                            RENDER_TARGET_DEFERRED_SPECPOW 
                        } 
                    ).
                    Set( SHADER_DEFERRED_DEPTH ).
                    Set( DEPTH_STENCIL_OVERDRAW ).
                    Set( DEPTH_STATE_DEPTH_NOSTENCIL ).
                    Set( SAMPLER_STATE_WRAP_TEX ).
                    Set( RASTER_STATE_SOLID_NOCULL ).
                    Set( BLEND_STATE_COUNT ).
                    Set( DXCore::TOPOLOGY_TRIANGLE_LIST ).

                    AddResource( &viewBuffer ).

                    Accepts( RENDERABLE_OVERDRAW ).
                    Processes( &modelProcessor ).
                InitializePass( );

                /////////////////////////////////////////////////////////
                // SPRITE TEXT PASS
                textPass.
                     Set(
                        {
                            RENDER_TARGET_SWAPCHAIN,
                            RENDER_TARGET_DEFERRED_NORMAL,
                            RENDER_TARGET_DEFERRED_SPECPOW
                        }
                    ).
                    Set( SHADER_SPRITE_TEXT ).
                    Set( DEPTH_STENCIL_MAIN ).
                    Set( DEPTH_STATE_DEPTH_NOSTENCIL ).
                    Set( SAMPLER_STATE_WRAP_TEX ).
                    Set( RASTER_STATE_SOLID_NOCULL ).
                    Set( BLEND_STATE_COUNT ).
                    Set( DXCore::TOPOLOGY_TRIANGLE_LIST ).

                    AddResource( &viewBuffer ).
                    AddResource( &particleModel ).
                    AddResource( &invView ).
                    AddResource( &fontTexture ).

                    Accepts( RENDERABLE_SPRITE_TEXT ).
                    Processes( &textProcessor ).
                    OverrideLayout(SHADER_OVERRIDE).
                InitializePass( );

                /////////////////////////////////////////////////////////
                // INIT PIPELINE
                MainPipeline.
                    AddPrePass( &forwardPass ).
                    AddPrePass( &billboardPass ).
                    AddPrePass( &overdrawPass ).
                    AddPrePass( &lineRenderPass ).
                    AddPrePass( &pointRenderPass ).
                    AddPrePass( &overdrawLinePass ).
                    AddPrePass( &overdrawPointPass ).
                    AddPrePass( &particlePass ).
                    AddPrePass( &textPass ).
                InitializePass( );
            }

            /////////////////////////////////////////////////////////
            // UPDATE GLOBAL RESOURCES
            CameraBuffer cb;
            PointGeometryBuffer pgb;
            TransformBuffer tb;
            invViewBuffer ipb;

            cb.view = SMat4::Transpose( currentCamera.GetViewMatrix( ) ).ToD3D( );
            cb.projection = SMat4::Transpose( currentCamera.GetProjMatrix( ) ).ToD3D( );

            viewBuffer.Update( cb );
            viewBufferGeom.Update( cb );
            spriteModel.Update( 0 );
            particleModel.Update( INTERNAL_POINT_INDICES );
            fontTexture.Update( 0 );

            pgb.cameraPosition = SVec4( currentCamera.GetPosition( ), 1 ).ToD3D( );
            pgb.cameraUp = SVec4( currentCamera.GetUp( ), 0 ).ToD3D( );
            geomBuff.Update( pgb );

            tb.transform = SMat4::Identity( ).ToD3D( );
            identity.Update( tb );

            SMat4 temp = currentCamera.GetViewMatrix( );
            temp.Transpose( );
            temp.Inverse( );
            ipb.invView = temp.ToD3D( );
            invView.Update( ipb, SHADER_SLOT_4 );

            /////////////////////////////////////////////////////////
            // EXECUTE RENDER CALL //////////////////////////////////
            MainPipeline.Execute( currentCamera );
        }

        void GfxManager::EndScene()
        {
            UAssert(m_currentlyRendering == true, "Attemped to end a scene without starting the frame!");
            UAssert(m_sceneActive == true, "Attempted to end a scene before beginning one!");
            m_sceneActive = false;

            // reset drawing for next scene
            drawingManager->EndScene();
        }

        void GfxManager::EndFrame()
        {
            UAssert(m_sceneActive == false, "Attempted to end the frame without ending the scene!");
            UAssert(m_currentlyRendering == true, "Attemped to end the frame when it was never started!");

            //compute pass for mouse position
            PrepForCompute();
            RenderComputeMousePos();

            m_currentlyRendering = false;

            dxCore->EndDebugEvent();

            // present
            dxCore->SwapChainBuffer();

            // end the frame
            gfxProfiler->EndFrame();

            // end profiler
            gfxProfiler->WaitForCalls(m_profile);

            //end rendering
            m_rendering = false;

            //check to resize
            dxCore->CheckSize();

            //invalidate CPU-side gfx engine for next frame
            dxCore->Invalidate();
            Invalidate();

        }

        void GfxManager::PrepForCompute(void)
        {
#if defined(URSINE_WITH_EDITOR)

            //set states
            dxCore->SetRenderTarget(RENDER_TARGET_LIGHTMAP, DEPTH_STENCIL_COUNT);
            dxCore->SetBlendState(BLEND_STATE_ADDITIVE);
            dxCore->SetDepthState(DEPTH_STATE_NODEPTH_NOSTENCIL);
            dxCore->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            //bind shader
            shaderManager->BindShader(SHADER_MOUSEPOSITION);
#endif
        }

        void GfxManager::PrepForUI()
        {
            SMat4 trans;
            trans.Translate(SVec3(0, 0, 0.1f));

            dxCore->SetBlendState(BLEND_STATE_DEFAULT);
            dxCore->SetRasterState(RASTER_STATE_UI);
            dxCore->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            dxCore->SetDepthState(DEPTH_STATE_NODEPTH_NOSTENCIL);
            dxCore->SetRenderTarget(RENDER_TARGET_SWAPCHAIN);
            bufferManager->MapCameraBuffer(SMat4::Identity(), SMat4::Identity());

            textureManager->MapSamplerState(SAMPLER_STATE_NO_FILTERING);
            shaderManager->BindShader(SHADER_UI);
            layoutManager->SetInputLayout(SHADER_UI);
            bufferManager->MapTransformBuffer(SMat4(-2, 2, 1) * trans);
            modelManager->BindModel( INTERNAL_QUAD );
        }

        void GfxManager::RenderComputeMousePos()
        {
            MouseBuffer dataToCS;

            POINT point;
            GetCursorPos(&point);

            ScreenToClient(wHND, &point);

            if ( point.x < 0 ) point.x = 0;
            if ( point.y < 0 ) point.y = 0;

            //@matt set proper mouse position
            dataToCS.mousePos = DirectX::XMINT4(point.x, point.y, 0, 0);

            //bind shader
            shaderManager->BindShader(SHADER_MOUSEPOSITION);

            //set input
            bufferManager->MapBuffer<BUFFER_MOUSEPOS>(&dataToCS, SHADERTYPE_COMPUTE, 0);
            dxCore->GetDeviceContext()->CSSetShaderResources(0, 1, &dxCore->GetRenderTargetMgr()->GetRenderTarget(RENDER_TARGET_DEFERRED_SPECPOW)->ShaderMap);
            ID3D11ShaderResourceView *srv = dxCore->GetDepthMgr()->GetDepthStencilSRV(DEPTH_STENCIL_MAIN);
            dxCore->GetDeviceContext()->CSSetShaderResources(1, 1, &srv);

            //set UAV as output 
            dxCore->GetDeviceContext()->CSSetUnorderedAccessViews(COMPUTE_BUFFER_ID, 1, &bufferManager->m_computeUAV[ COMPUTE_BUFFER_ID ], nullptr);
            dxCore->GetDeviceContext()->CSSetUnorderedAccessViews(COMPUTE_BUFFER_ID_CPU, 1, &bufferManager->m_computeUAV[ COMPUTE_BUFFER_ID_CPU ], nullptr);

            //call the compute shader. Results *should* be written to the UAV above
            dxCore->GetDeviceContext()->Dispatch(1, 1, 1);

            //copy data to intermediary buffer
            //                                                           CPU read-only staging buffer                            GPU compute output
            dxCore->GetDeviceContext()->CopyResource(bufferManager->m_computeBufferArray[ COMPUTE_BUFFER_ID_CPU ], bufferManager->m_computeBufferArray[ COMPUTE_BUFFER_ID ]);

            //read from intermediary buffer
            ComputeIDOutput dataFromCS;
            bufferManager->ReadComputeBuffer<COMPUTE_BUFFER_ID_CPU>(&dataFromCS, SHADERTYPE_COMPUTE);

            dxCore->GetDeviceContext()->CSSetShaderResources(0, 0, nullptr);

            m_currentPosition = SVec3(static_cast<float>(point.x), static_cast<float>(point.y), dataFromCS.depth);

            tempID = dataFromCS.id;

            int index = tempID & 0x7FF;
            int type = (tempID >> 12) & 0x3;
            int overdraw = (tempID >> 15) & 0x1;

            if (index >= 512)
                return;

            unsigned w, h;
            gfxInfo->GetDimensions(w, h);

            if ( tempID != -1 && tempID < 73727 && (unsigned)point.x < w && (unsigned)point.y < h )
            {
                switch ( type )
                {
                case RENDERABLE_MODEL3D:
                case 3: // work around for overdraw, since bitfield only holds 4 values
                    m_currentID = renderableManager->m_renderableModel3D[ index ].GetEntityID( );
                    break;
                case RENDERABLE_BILLBOARD2D:
                    m_currentID = renderableManager->m_renderableBillboards[ index ].GetEntityID( );
                    break;
                case RENDERABLE_SPRITE_TEXT:
                    m_currentID = renderableManager->m_renderableSpriteText[ index ].GetEntityID( );
                    break;
                }
            }
            else
                m_currentID = -1;
        }

        void GfxManager::SetGameViewport(GfxHND vp)
        {
            _VIEWPORTHND *viewPort = reinterpret_cast<_VIEWPORTHND*>(&vp);

            UAssert(viewPort->ID_ == ID_VIEWPORT, "Attempted to set game viewport with invalid handle!");

            m_GameViewport = vp;
        }

        void GfxManager::RenderDynamicTexture(GfxHND& texHandle, const float posX, const float posY)
        {
            //get the texture
            Texture *tex = textureManager->GetDynamicTexture(texHandle);

            _RESOURCEHND *handle = HND_RSRCE(texHandle);

            //prep for ui
            PrepForUI();

            //get dimensions
            unsigned width, height;
            gfxInfo->GetDimensions(width, height);

            //will need these to be floats
            float fWidth = static_cast<float>(width);
            float fHeight = static_cast<float>(height);

            //set directx viewport
            D3D11_VIEWPORT vpData = viewportManager->GetViewport(m_GameViewport).GetViewportData();
            unsigned w, h;
            gfxInfo->GetDimensions(w, h);
            vpData.TopLeftX = 0;
            vpData.TopLeftY = 0;
            vpData.Width = static_cast<FLOAT>(w);
            vpData.Height = static_cast<FLOAT>(h);

            dxCore->GetDeviceContext()->RSSetViewports(1, &vpData);

            //calculate position w/ respect to top left
            float finalX = (posX - fWidth / 2.f + tex->m_width / 2.f) / (fWidth / 2.f);
            float finalY = (-posY + height / 2.f - tex->m_height / 2.f) / (fHeight / 2.f);
            SMat4 trans = SMat4(SVec3(finalX, finalY, 0));
            bufferManager->MapTransformBuffer(trans * SMat4(-2 * (tex->m_width / fWidth), 2 * (tex->m_height / fHeight), 1));

            //map tex
            textureManager->MapTextureByID(handle->Index_);

            //render to screen
            shaderManager->Render( modelManager->GetModelIndexcountByID( INTERNAL_QUAD ) );
        }

        void GfxManager::RenderDynamicTextureInViewport(GfxHND& texHandle, const float posX, const float posY, GfxHND& camera)
        {
            _RESOURCEHND *newRender = reinterpret_cast<_RESOURCEHND*>(&camera);

            UAssert(newRender->ID_ == ID_CAMERA, "Attempted to render UI with invalid camera!");

            // get viewport
            Camera &cam = cameraManager->GetCamera(camera);

            //get the texture
            Texture *tex = textureManager->GetDynamicTexture(texHandle);

            _RESOURCEHND *handle = HND_RSRCE(texHandle);

            //prep for ui
            PrepForUI();

            //get dimensions
            unsigned width, height;
            gfxInfo->GetDimensions(width, height);

            //will need these to be floats
            float fWidth = static_cast<float>(width);
            float fHeight = static_cast<float>(height);

            //set directx viewport
            float w, h, x, y;
            Viewport &gameVP = viewportManager->GetViewport(m_GameViewport);
            D3D11_VIEWPORT gvp = gameVP.GetViewportData();
            cam.GetViewportPosition(x, y);
            cam.GetDimensions(w, h);

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

            dxCore->GetDeviceContext()->RSSetViewports(1, &vpData);

            //calculate position w/ respect to top left
            float finalX = (posX - fWidth / 2.f + tex->m_width / 2.f) / (fWidth / 2.f);
            float finalY = (-posY + height / 2.f - tex->m_height / 2.f) / (fHeight / 2.f);
            SMat4 trans = SMat4(SVec3(finalX, finalY, 0));
            bufferManager->MapTransformBuffer(trans * SMat4(-2 * (tex->m_width / fWidth), 2 * (tex->m_height / fHeight), 1));

            //map tex
            textureManager->MapTextureByID(handle->Index_);

            //render to screen
            shaderManager->Render( modelManager->GetModelIndexcountByID( INTERNAL_QUAD ) );
        }

        void GfxManager::RenderToDynamicTexture(const int srcWidth, const int srcHeight, const void* input, const int inputWidth, const int inputHeight, GfxHND destTexture, const int destinationX, const int destinationY)
        {
            //set up description
            D3D11_TEXTURE2D_DESC desc;
            desc.Width = srcWidth;
            desc.Height = srcHeight;
            desc.MipLevels = desc.ArraySize = 1;
            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // dammmnnnn
            desc.SampleDesc.Count = 1;
            desc.SampleDesc.Quality = 0;
            desc.Usage = D3D11_USAGE_DEFAULT;
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            desc.CPUAccessFlags = 0;
            desc.MiscFlags = 0;

            //set up resource
            D3D11_SUBRESOURCE_DATA subrsc;
            subrsc.pSysMem = input;
            subrsc.SysMemPitch = srcWidth * 4; //length of one line in bytes, 32 bit color
            subrsc.SysMemSlicePitch = 0;

            //create the texture
            ID3D11Texture2D *tex;
            HRESULT hr = dxCore->GetDevice()->CreateTexture2D(&desc, &subrsc, &tex);

            UAssert(hr == S_OK, "Failed to create UI texture!");

            //this doesn't work for now
            //define the box of the texture
            D3D11_BOX box; //this box is the taken from the SOURCE texture
            box.back = 1; //this might need SERIOUS changes
            box.front = 0;

            box.left = 0;
            box.top = 0;

            box.right = inputWidth;
            box.bottom = inputHeight;

            //now that we have the texture, we need to write it to the render target
            Texture *target = textureManager->GetDynamicTexture(destTexture);
            dxCore->GetDeviceContext()->CopySubresourceRegion(target->m_texture2d, 0, destinationX, destinationY, 0, tex, 0, &box);


            RELEASE_RESOURCE(tex);
        }

        int GfxManager::GetCurrentUniqueID()
        {
            return m_currentID;
        }

        SVec3 GfxManager::GetCurrentWorldPosition(const GfxHND& cameraHandle)
        {
            auto &camera = cameraManager->GetCamera(cameraHandle);

            // get the saved depth
            float depth = m_currentPosition.Z();

            // transform from screen to world, given a specific camera
            auto worldPosition = camera.ScreenToWorld(Vec2(m_currentPosition.X(), m_currentPosition.Y()), depth);

            return worldPosition;
        }

        // misc stuff /////////////////////////////////////////////////////
        DXCore::DirectXCore *GfxManager::GetDXCore()
        {
            return dxCore;
        }

        void GfxManager::Resize(int width, int height)
        {
            if ( !m_ready )
                return;

            gfxInfo->SetDimensions(width, height);

            //MAIN render targets, not viewports
            dxCore->ResizeDX(width, height);

            Invalidate();
        }

        void GfxManager::SetFullscreenState(const bool state)
        {
            dxCore->SetFullscreenState(state);
        }

        void GfxManager::Invalidate()
        {
            dxCore->Invalidate();
            shaderManager->Invalidate();
            layoutManager->Invalidate();
        }

        void GfxManager::RenderUI(GfxHND camera, RENDER_TARGETS input)
        {
            _RESOURCEHND *newRender = reinterpret_cast<_RESOURCEHND*>(&camera);

            UAssert(newRender->ID_ == ID_CAMERA, "Attempted to render UI with invalid camera!");

           

            // get viewport
            Camera &cam = cameraManager->GetCamera(camera);

            //get game vp dimensions
            Viewport &gameVP = viewportManager->GetViewport(m_GameViewport);
            D3D11_VIEWPORT gvp = gameVP.GetViewportData();

            //set directx viewport
            float w, h, x, y;
            cam.GetViewportPosition(x, y);
            cam.GetDimensions(w, h);

            PrepForUI();

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

            dxCore->GetDeviceContext()->RSSetViewports(1, &vpData);

            dxCore->GetDeviceContext()->PSSetShaderResources(0, 1, &dxCore->GetRenderTargetMgr()->GetRenderTarget(input)->ShaderMap);
            dxCore->SetRenderTarget(RENDER_TARGET_SWAPCHAIN);
            shaderManager->Render( modelManager->GetModelIndexcountByID( INTERNAL_QUAD ) );
        }

        void GfxManager::RenderUI_Main(RENDER_TARGETS input)
        {
            //set directx viewport
            D3D11_VIEWPORT vpData = viewportManager->GetViewport(m_GameViewport).GetViewportData();
            unsigned w, h;
            gfxInfo->GetDimensions(w, h);
            vpData.TopLeftX = 0;
            vpData.TopLeftY = 0;
            vpData.Width = static_cast<FLOAT>(w);
            vpData.Height = static_cast<FLOAT>(h);

            dxCore->GetDeviceContext()->RSSetViewports(1, &vpData);

            PrepForUI();

            dxCore->GetDeviceContext()->PSSetShaderResources(0, 1, &dxCore->GetRenderTargetMgr()->GetRenderTarget(input)->ShaderMap);
            dxCore->SetRenderTarget(RENDER_TARGET_SWAPCHAIN);
            shaderManager->Render( modelManager->GetModelIndexcountByID( INTERNAL_QUAD ) );
        }
    }
}
