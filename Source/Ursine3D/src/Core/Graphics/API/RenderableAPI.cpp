/* ---------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
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
            return m_privates->renderMgr->GetRenderable<Model3D>(handle);
        }

        Billboard2D &RenderableAPI::GetBillboard2D(GfxHND &handle)
        {
            return m_privates->renderMgr->GetRenderable<Billboard2D>(handle);
        }

        Light& RenderableAPI::GetLight(GfxHND &handle)
        {
            return m_privates->renderMgr->GetRenderable<Light>(handle);
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