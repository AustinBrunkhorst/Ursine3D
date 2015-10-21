#include "UrsinePrecompiled.h"
#include "GfxManager.h"
#include "GfxAPI.h"

namespace ursine
{
    namespace graphics
    {
        CORE_SYSTEM_DEFINITION(GfxAPI);

        //privates
        struct GfxAPI::privateData
        {
            GfxManager *gfxCore_;
        };

        GfxAPI::GfxAPI(void)
            : m_privates(nullptr)
        {

        }

        GfxAPI::~GfxAPI(void)
        {
            if (m_privates == nullptr)
                return;
            
            //uninit API managers
            ResourceMgr.Uninitialize();
            RenderableMgr.Uninitialize();
            CameraMgr.Uninitialize();
            ViewportMgr.Uninitialize();
            UIMgr.Uninitialize();
            DrawingMgr.Uninitialize();

            //uninit graphics as a whole
            m_privates->gfxCore_->Uninitialize();

            //delete everything
            delete m_privates->gfxCore_;
            delete m_privates;
        }

        void GfxAPI::OnInitialize(void)
        {

        }

        void GfxAPI::OnRemove(void)
        {

        }

        void GfxAPI::StartGraphics(GfxConfig config)
        {
            //create privates
            m_privates = new privateData;

            //create gfx
            m_privates->gfxCore_ = new GfxManager();
            m_privates->gfxCore_->Initialize(config);

            //create APIs
            ResourceMgr.Initialize();
            ResourceMgr.SetPrivates(m_privates->gfxCore_->modelManager, m_privates->gfxCore_->textureManager);
            RenderableMgr.Initialize();
            RenderableMgr.SetPrivates(m_privates->gfxCore_->renderableManager);
            CameraMgr.Initialize();
            CameraMgr.SetPrivates(m_privates->gfxCore_->cameraManager);
            ViewportMgr.Initialize();
            ViewportMgr.SetPrivates(m_privates->gfxCore_->viewportManager);
            UIMgr.Initialize();
            UIMgr.SetPrivates(m_privates->gfxCore_->uiManager);
            DrawingMgr.Initialize();
            DrawingMgr.SetPrivates(m_privates->gfxCore_->drawingManager);
        }

        void GfxAPI::StartFrame()
        {
            m_privates->gfxCore_->StartFrame();
        }

        void GfxAPI::RenderObject(GfxHND handle)
        {
            m_privates->gfxCore_->Render(handle);
        }

        void GfxAPI::BeginScene()
        {
            m_privates->gfxCore_->BeginScene();
        }

        void GfxAPI::RenderScene(float dt, GfxHND camera)
        {
            m_privates->gfxCore_->RenderScene(dt, camera);
        }

        void GfxAPI::EndScene()
        {
            m_privates->gfxCore_->EndScene();
        }

        void GfxAPI::EndFrame()
        {
            m_privates->gfxCore_->EndFrame();
        }

        void GfxAPI::Resize(int width, int height)
        {
            m_privates->gfxCore_->Resize(width, height);
        }

        void GfxAPI::SetGameViewport(GfxHND vp)
        {
            m_privates->gfxCore_->SetGameViewport(vp);
        }

        void GfxAPI::RenderTextureMain(GfxHND& handle, const float posX, const float posY)
        {
            m_privates->gfxCore_->RenderDynamicTexture(handle, posX, posY);
        }

        void GfxAPI::RenderTextureInViewport(GfxHND& handle, const float posX, const float posY, GfxHND &camera)
        {
            m_privates->gfxCore_->RenderDynamicTextureInViewport(handle, posX, posY, camera);
        }

        void GfxAPI::RenderToDynamicTexture(const int srcWidth, const int srcHeight, 
                                            const void* input, 
                                            const int inputWidth, const int inputHeight, 
                                            GfxHND destTexture, 
                                            const int destinationX, const int destinationY)
        {
            m_privates->gfxCore_->RenderToDynamicTexture(srcWidth, srcHeight, input, inputWidth, inputHeight, destTexture, destinationX, destinationX);
        }

        ursine::ecs::EntityUniqueID GfxAPI::GetMousedOverID()
        {
            return m_privates->gfxCore_->GetCurrentUniqueID();
        }
    }
}
