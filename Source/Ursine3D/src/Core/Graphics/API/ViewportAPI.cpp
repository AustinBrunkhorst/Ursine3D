/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ViewportAPI.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ViewportAPI.h"
#include "ViewportManager.h"

namespace ursine
{
    namespace graphics
    {
        struct ViewportAPI::privData
        {
            ViewportManager *mgr;
        };

        GfxHND ViewportAPI::CreateViewport(unsigned width, unsigned height)
        {
            return m_privates->mgr->AddViewport(width, height);
        }

        void ViewportAPI::DestroyViewport(GfxHND &handle)
        {
            m_privates->mgr->DestroyViewport(handle);
        }

        Viewport &ViewportAPI::GetViewport(GfxHND &handle)
        {
            return m_privates->mgr->GetViewport(handle);
        }

        void ViewportAPI::SetPrivates(void *priv)
        {
            m_privates->mgr = reinterpret_cast<ViewportManager*>(priv);
        }

        void ViewportAPI::Initialize()
        {
            m_privates = new privData;
        }

        void ViewportAPI::Uninitialize() { }
    }
}