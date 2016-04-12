/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RenderableAPI.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

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
            return m_privates->renderMgr->GetCurrentRenderableByID<Model3D>(static_cast<unsigned>(handle & 0xFFFF));
        }

        Billboard2D &RenderableAPI::GetBillboard2D(GfxHND &handle)
        {
            return m_privates->renderMgr->GetCurrentRenderableByID<Billboard2D>(static_cast<unsigned>(handle & 0xFFFF));
        }

        Light &RenderableAPI::GetLight(GfxHND &handle)
        {
            return m_privates->renderMgr->GetCurrentRenderableByID<Light>(static_cast<unsigned>(handle & 0xFFFF));
        }

        ParticleSystem &RenderableAPI::GetParticleSystem(GfxHND & handle)
        {
            return m_privates->renderMgr->GetCurrentRenderableByID<ParticleSystem>(static_cast<unsigned>(handle & 0xFFFF));
        }

        void RenderableAPI::SetPrivates(void *priv)
        {
            m_privates->renderMgr = reinterpret_cast<RenderableManager*>(priv);
        }

        void RenderableAPI::Initialize(void)
        {
            m_privates = new RenderableAPI::privData;
        }

        void RenderableAPI::Uninitialize(void)
        {
            delete m_privates;
        }

        SpriteText& RenderableAPI::GetSpriteText(GfxHND& handle)
        {
            return m_privates->renderMgr->GetCurrentRenderableByID<SpriteText>(static_cast<unsigned>(handle & 0xFFFF));
        }
    }
}