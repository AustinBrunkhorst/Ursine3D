#include "UrsinePrecompiled.h"
#include "RenderableAPI.h"
#include "RenderableManager.h"

namespace ursine
{
    struct RenderableAPI::privData
    {
        RenderableManager *renderMgr;
    };

    GFXHND RenderableAPI::AddRenderable(const RENDERABLE_TYPE type)
    {
        return m_privates->renderMgr->AddRenderable( type );
    }

    void RenderableAPI::DestroyRenderable(GFXHND &handle)
    {
        m_privates->renderMgr->DestroyRenderable( handle );
    }

    Model3D &RenderableAPI::GetModel3D(GFXHND &handle)
    {
        return m_privates->renderMgr->GetRenderable<Model3D>( handle );
    }

    DirectionalLight &RenderableAPI::GetDirectionalLight(GFXHND &handle)
    {
        return m_privates->renderMgr->GetRenderable<DirectionalLight>( handle );
    }

    PointLight &RenderableAPI::GetPointLight(GFXHND &handle)
    {
        return m_privates->renderMgr->GetRenderable<PointLight>( handle );
    }

    Primitive &RenderableAPI::GetPrimitive(GFXHND &handle)
    {
        return m_privates->renderMgr->GetRenderable<Primitive>( handle );
    }

    Billboard2D &RenderableAPI::GetBillboard2D(GFXHND &handle)
    {
        return m_privates->renderMgr->GetRenderable<Billboard2D>( handle );
    }

    void RenderableAPI::SetPrivates(void *priv)
    {
        m_privates->renderMgr = reinterpret_cast<RenderableManager*>( priv );
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
