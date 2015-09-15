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

    //private methods
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

    static DWORD WINAPI renderBootstrap( LPVOID lpParam );
    void RenderScene_Forward( float dt, GFXHND viewport = -1 );
    void RenderScene_Deferred( float dt, GFXHND viewport = -1 );

    //privates members
  private:
    std::atomic<bool> m_rendering;

    bool m_renderUI;
    bool m_profile;
    std::vector<DRAWHND> m_drawList;
    unsigned m_drawCount;

    //temp
    std::list<GFXHND> m_viewportList;
  };
}