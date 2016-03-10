/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GfxManager.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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
#include "BitmapFont.h"
#include "FontManager.h"

#include <atomic>

namespace ursine
{
    namespace graphics
    {
        class GfxManager
        {
            friend class RenderPass;
            friend class GlobalShaderResource;

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

            void SetFullscreenState(const bool state);

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

            // get the currently selected object
            int GetCurrentUniqueID();

            SVec3 GetCurrentWorldPosition(const GfxHND &cameraHandle);

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
            FontManager *fontManager;
            GfxProfiler *gfxProfiler;

            GfxInfo *gfxInfo;

            BitmapFont m_font;

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
            void PrepForCompute(void);
            void PrepForUI(void);

            //rendering funcs
            void RenderComputeMousePos(void);

            //privates members
        private:
            std::atomic<bool> m_rendering;

            bool m_sceneActive; //was beginscene called
            bool m_currentlyRendering;//was start frame called
            bool m_renderUI;
            bool m_profile;
            bool m_debug;
            bool m_ready = false;

            int m_currentID;            // current object we are moused over
            SVec3 m_currentPosition;    // view position of current object, stored in x pos, y pos, depth

            GfxHND m_GameViewport;

            std::vector<_DRAWHND> m_drawList;
            unsigned m_drawCount;

            //temp
            std::list<GfxHND> m_viewportList;
        };
    }
}
