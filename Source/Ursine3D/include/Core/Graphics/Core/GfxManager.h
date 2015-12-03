/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      GfxManager.h
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
    namespace graphics
    {
        class GfxManager
        {
            //public methods
        public:
            void Initialize(GfxConfig &config);
            void Uninitialize();

            void Render(GfxHND handle);

            void StartFrame();

            void BeginScene();

            void RenderScene(float dt, GfxHND viewport = -1);

            void EndScene();

            void EndFrame();

            DXCore::DirectXCore *GetDXCore();

            void Resize(int width, int height);

            void SetFullscreenState( const bool state );

            void Invalidate();

            void RenderUI(GfxHND camera, RENDER_TARGETS input);

            void RenderUI_Main(RENDER_TARGETS input);

            //set the viewport for the current game
            void SetGameViewport(GfxHND vp);

            //render texture to screen
            void RenderDynamicTexture(GfxHND &texHandle, const float posX, const float posY);

            //render in cam viewport
            void RenderDynamicTextureInViewport(GfxHND &texHandle, const float posX, const float posY, GfxHND &camera);

            //render onto a texture
            void RenderToDynamicTexture(const int srcWidth, const int srcHeight, const void *input, const int inputWidth, const int inputHeight, GfxHND destTexture, const int destinationX, const int destinationY);

            int GetCurrentUniqueID();

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
                GfxManager *gfx;
                float dt;
                bool forward;
                GfxHND viewport;
            };

            HANDLE m_threadHandle;
            DWORD m_threadID;

            //private methods
        private:
            static DWORD WINAPI renderBootstrap(LPVOID lpParam);
            void RenderScene_Forward(float dt, GfxHND viewport = -1);
            void RenderScene_Deferred(float dt, GfxHND viewport = -1);

            //preparing for rendering
            void PrepFor3DModels(const SMat4 &view, const SMat4 &proj);
            void PrepForBillboard2D(const SMat4 &view, const SMat4 &proj, Camera &currentCamera);
            void PrepForCompute(void);
            void PrepForPointLightPass(const SMat4 &view, const SMat4 &proj);
            void PrepForSpotlightPass(const SMat4 &view, const SMat4 &proj);
            void PrepForDirectionalLightPass(const SMat4 &view, const SMat4 &proj);
            void PrepForPrimitives(const SMat4 &view, const SMat4 &proj);
            void PrepForDebugRender(void);
            void PrepForFinalOutput(void);
            void PrepForUI(void);
            void PrepForOverdrawDebugRender(const SMat4 &view, const SMat4 &proj);

            //rendering funcs
            void Render3DModel(_DRAWHND handle, Camera &currentcamera );
            void Render2DBillboard(_DRAWHND handle, Camera &currentCamera);
            void RenderComputeMousePos(void);
            void RenderPointLight(_DRAWHND handle, Camera &currentCamera, SMat4 &proj);
            void RenderSpotLight(_DRAWHND handle, Camera &currentCamera, SMat4 &proj);
            void RenderDirectionalLight(_DRAWHND handle, Camera &currentcamera);
            void RenderPrimitive(_DRAWHND handle, Camera &currentcamera );
            void RenderDebugPoints(const SMat4 &view, const SMat4 &proj, Camera &currentCamera, bool overdraw = false);
            void RenderDebugLines(const SMat4 &view, const SMat4 &proj, Camera &currentCamera, bool overdraw = false);

            //privates members
        private:
            std::atomic<bool> m_rendering;

            bool m_sceneActive; //was beginscene called
            bool m_currentlyRendering;//was start frame called
            bool m_renderUI;
            bool m_profile;
            bool m_debug;
            bool m_ready = false;

            int m_currentID;    //current object we are moused over

            GfxHND m_GameViewport;

            std::vector<_DRAWHND> m_drawList;
            unsigned m_drawCount;

            //temp
            std::list<GfxHND> m_viewportList;
        };
    }
}
