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
            friend class ShadowPass;
            friend class GlobalShaderResource;

            //public methods
        public:
            void Initialize(GfxConfig &config);
            void Uninitialize();

            // methods for rendering
            void Render(GfxHND handle);

            void StartFrame();

            void BeginScene();

            void RenderScene(float dt, GfxHND viewport = -1);

            void EndScene();

            void EndFrame();

            // utility methods
            DXCore::DirectXCore *GetDXCore();

            void Resize(int width, int height);

            void SetFullscreenState(const bool state);

            void Invalidate();

            //set the viewport for the current game
            void SetGameViewport(GfxHND vp);

            //render texture to screen
            void RenderDynamicTexture(GfxHND &texHandle, const float posX, const float posY);

            //render in cam viewport
            void RenderDynamicTextureInViewport(GfxHND &texHandle, const float posX, const float posY, GfxHND &camera);

            //render onto a texture
            void RenderToDynamicTexture(const int srcWidth, const int srcHeight, const void *input, const int inputWidth, const int inputHeight, GfxHND destTexture, const int destinationX, const int destinationY);

            // get the currently selected object
            int GetCurrentUniqueID(void);

            SVec3 GetCurrentWorldPosition(const GfxHND &cameraHandle);

            //public members
        public:
            DXCore::DirectXCore         *dxCore;
            DXCore::ShaderManager       *shaderManager;
            DXCore::ShaderBufferManager *bufferManager;

            DXCore::InputLayoutManager  *layoutManager;
            ModelManager                *modelManager;
            RenderableManager           *renderableManager;
            CameraManager               *cameraManager;
            TextureManager              *textureManager;
            ViewportManager             *viewportManager;
            GfxUIManager                *uiManager;
            DrawingManager              *drawingManager;
            FontManager                 *fontManager;
            GfxProfiler                 *gfxProfiler;

            GfxInfo                     *gfxInfo;

            float m_lightSteps;
            float m_borderValue;
            float m_globalEmissive;
            GfxHND m_lightMapTexture;

        private:
            struct UIRenderData
            {
                GfxHND texHandle;
                float posX;
                float posY;
                GfxHND cameraHandle;
            };

        private:
            // multithread rendering methods
            static void internalGfxEntry(GfxManager *manager);

            void internalStartFrame(void);
            void internalRenderScene(Camera &camera, int index);
            void internalEndFrame(void);

            void RenderScene_Deferred(Camera &camera, int index);
            void RenderScene_Forward(Camera &camera, int index);
            
            // misc methods
            void internalRenderDynamicTexture(GfxHND &texHandle, const float posX, const float posY);
            void internalRenderDynamicTextureInViewport(GfxHND &texHandle, const float posX, const float posY, GfxHND &camera);
            void prepForCompute(void);
            void prepForUI(void);
            void renderComputeMousePos(void);

            //privates members
        private:
            // keeping track of a bunch of states
            std::atomic<bool> m_rendering;

            bool m_sceneActive; //was beginscene called
            bool m_currentlyRendering;//was start frame called
            bool m_renderUI;
            bool m_profile;
            bool m_debug;
            bool m_ready = false;

            int m_currentID;            // current object we are moused over
            SVec3 m_currentPosition;    // view position of current object, stored in x pos, y pos, depth

            // main game viewport
            GfxHND m_GameViewport;

            // ui render calls
            std::vector<UIRenderData>                   m_uiRenderCalls;
            std::vector<UIRenderData>                   m_uiViewportRenderCalls;

            // list of cameras for this frame
            std::vector<std::pair<Camera, unsigned>>    m_cameraList;

            // list of render calls for each from
            std::vector<std::vector<_DRAWHND>>          m_drawLists;
            std::vector<unsigned>                       m_drawCounts;
            int                                         m_currentList;

            // thread object
            std::thread m_gfxThread;
        };
    }
}
