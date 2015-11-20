/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CameraAPI.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "CameraAPI.h"
#include "CameraManager.h"

namespace ursine
{
    namespace graphics
    {
        struct CameraAPI::privData
        {
            CameraManager *mgr;
        };

        GfxHND CameraAPI::AddCamera()
        {
            return m_privates->mgr->CreateCamera();
        }

        Camera &CameraAPI::GetCamera(const GfxHND &handle)
        {
            return m_privates->mgr->GetCamera(handle);
        }

        void CameraAPI::DestroyCamera(GfxHND &handle)
        {
            m_privates->mgr->DestroyCamera(handle);
        }

        void CameraAPI::SetPrivates(void *priv)
        {
            m_privates->mgr = reinterpret_cast<CameraManager*>(priv);
        }

        void CameraAPI::Initialize()
        {
            m_privates = new CameraAPI::privData();
        }

        void CameraAPI::Uninitialize()
        {
            delete m_privates;
        }
    }
}
