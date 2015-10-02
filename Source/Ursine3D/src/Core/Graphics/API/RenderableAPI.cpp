#include "UrsinePrecompiled.h"
#include "RenderableAPI.h"
#include "RenderableManager.h"

namespace ursine
{
    namespace graphics
    {
        struct RenderableAPI::privData
        {
            RenderableManager *renderMgr;
        };

        GfxHND RenderableAPI::AddRenderable(const RENDERABLE_TYPE type)
        {
            return m_privates->renderMgr->AddRenderable(type);
        }

        void RenderableAPI::DestroyRenderable(GfxHND &handle)
        {
            m_privates->renderMgr->DestroyRenderable(handle);
        }

        Model3D &RenderableAPI::GetModel3D(GfxHND &handle)
        {
            return m_privates->renderMgr->GetRenderable<Model3D>(handle);
        }

        DirectionalLight &RenderableAPI::GetDirectionalLight(GfxHND &handle)
        {
            return m_privates->renderMgr->GetRenderable<DirectionalLight>(handle);
        }

        PointLight &RenderableAPI::GetPointLight(GfxHND &handle)
        {
            return m_privates->renderMgr->GetRenderable<PointLight>(handle);
        }

        Primitive &RenderableAPI::GetPrimitive(GfxHND &handle)
        {
            return m_privates->renderMgr->GetRenderable<Primitive>(handle);
        }

        Billboard2D &RenderableAPI::GetBillboard2D(GfxHND &handle)
        {
            return m_privates->renderMgr->GetRenderable<Billboard2D>(handle);
        }

        void RenderableAPI::SetPrivates(void *priv)
        {
            m_privates->renderMgr = reinterpret_cast<RenderableManager*>(priv);
        }

        void RenderableAPI::Initialize()
        {
            m_privates = new RenderableAPI::privData;
        }

        void RenderableAPI::Uninitialize()
        {
            delete m_privates;
        }
    }
}