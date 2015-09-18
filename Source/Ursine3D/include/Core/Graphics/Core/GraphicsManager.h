/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      GraphicsManager.h
Module:         Graphics
Purpose:        Core of graphics
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/
#pragma once

#include "DirectXCore.h"
#include "ShaderManager.h"
#include "ShaderBufferManager.h"
#include "InputLayoutManager.h"
#include "ModelManager.h"
#include "RenderableManager.h"
#include "GfxConfig.h"
#include "CameraManager.h"
#include "TextureManager.h"
#include "ViewportManager.h"
#include "GfxUIManager.h"
#include "DrawingManager.h"
#include "GfxProfiler.h"

#include <atomic>

namespace ursine
{
  class GraphicsCore
  {
    //public methods
  public:
    void Initialize( GfxConfig &config );
    void Uninitialize( );

    void Render( GFXHND handle );

    void BeginScene( );

    void RenderScene( float dt, GFXHND viewport = -1 );

    void EndScene( );

    DXCore::DirectXCore *GetDXCore( );

    void Resize( int width, int height );

    //public members
  public:
    DXCore::DirectXCore *dxCore;
    DXCore::ShaderManager *shaderManager;
    DXCore::ShaderBufferManager *bufferManager;
    DXCore::InputLayoutManager *layoutManager;
    ModelManager *modelManager;
    RenderableManager *renderableManager;
    CameraManager *cameraManager;
    TextureManager *textureManager;
    ViewportManager *viewportManager;
    GfxUIManager *uiManager;
    DrawingManager *drawingManager;
    GfxProfiler *gfxProfiler;

    GfxInfo *gfxInfo;

    //thred stuff
  private:
    struct threadData
    {
      GraphicsCore *gfx;
      float dt;
      bool forward;
      GFXHND viewport;
    };

    HANDLE m_threadHandle;
    DWORD m_threadID;

    //private methods
  private:
    static DWORD WINAPI renderBootstrap( LPVOID lpParam );
    void RenderScene_Forward( float dt, GFXHND viewport = -1 );
    void RenderScene_Deferred( float dt, GFXHND viewport = -1 );

    //preparing for rendering
    void PrepFor3DModels( DirectX::XMMATRIX &view, DirectX::XMMATRIX &proj );
    void PrepForLightPass( DirectX::XMMATRIX &view, DirectX::XMMATRIX &proj );
    void PrepForPrimitives( DirectX::XMMATRIX &view, DirectX::XMMATRIX &proj );
    void PrepForDebugRender( );
    void PrepForFinalOutput( );
    void PrepForUI( );

    //rendering funcs
    void Render3DModel( DRAWHND handle );
    void RenderPointLight( DRAWHND handle, Camera &currentCamera );
    void RenderDirectionalLight( DRAWHND handle, Camera &currentcamera );
    void RenderPrimitive( DRAWHND handle );
    void RenderDebugPoints( DirectX::XMMATRIX &view, DirectX::XMMATRIX &proj, Camera &currentCamera );
    void RenderDebugLines( DirectX::XMMATRIX &view, DirectX::XMMATRIX &proj, Camera &currentCamera );

    //privates members
  private:
    std::atomic<bool> m_rendering;

    bool m_renderUI;
    bool m_profile;
    bool m_debug;
    bool m_ready = false;
    std::vector<DRAWHND> m_drawList;
    unsigned m_drawCount;

    //temp
    std::list<GFXHND> m_viewportList;
  };
}