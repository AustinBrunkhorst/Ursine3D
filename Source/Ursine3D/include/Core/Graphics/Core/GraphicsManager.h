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

    void StartFrame( );

    void BeginScene( );

    void RenderScene( float dt, GFXHND viewport = -1 );

    void EndScene( );

    void EndFrame( );

    DXCore::DirectXCore *GetDXCore( );

    void Resize( int width, int height );

    void Invalidate( );

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
    void PrepFor3DModels( const SMat4 &view, const SMat4 &proj );
    void PrepForLightPass( const SMat4 &view, const SMat4 &proj );
    void PrepForPrimitives( const SMat4 &view, const SMat4 &proj );
    void PrepForDebugRender( );
    void PrepForFinalOutput( );
    void PrepForUI( );

    //rendering funcs
    void Render3DModel( _DRAWHND handle );
    void Render2DBillboard( _DRAWHND handle );
    void RenderPointLight( _DRAWHND handle, Camera &currentCamera );
    void RenderDirectionalLight( _DRAWHND handle, Camera &currentcamera );
    void RenderPrimitive( _DRAWHND handle );
    void RenderDebugPoints( const SMat4 &view, const SMat4 &proj, Camera &currentCamera );
    void RenderDebugLines( const SMat4 &view, const SMat4 &proj, Camera &currentCamera );

    //privates members
  private:
    std::atomic<bool> m_rendering;

    bool m_sceneActive;       //was beginscene called
    bool m_currentlyRendering;//was start frame called
    bool m_renderUI;
    bool m_profile;
    bool m_debug;
    bool m_ready = false;
    std::vector<_DRAWHND> m_drawList;
    unsigned m_drawCount;

    //temp
    std::list<GFXHND> m_viewportList;
  };
}