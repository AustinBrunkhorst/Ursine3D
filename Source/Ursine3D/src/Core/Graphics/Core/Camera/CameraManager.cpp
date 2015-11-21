/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CameraManager.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "CameraManager.h"
#include "GfxDefines.h"

namespace ursine
{
    namespace graphics
    {
        void CameraManager::Initialize(void)
        {
            m_cameraArray.resize(MAX_CAMERAS);

            for (unsigned x = 0; x < MAX_CAMERAS; ++x)
            {
                m_freeCameraList.push_back(x);
            }
        }

        void CameraManager::Uninitialize(void)
        {

        }

        GfxHND CameraManager::CreateCamera()
        {
            UAssert(m_freeCameraList.size() > 0, "Out of cameras!");
            GfxHND data;
            _RESOURCEHND *newRender = reinterpret_cast<_RESOURCEHND*>(&data);
            newRender->ID_ = ID_CAMERA;
            newRender->Index_ = m_freeCameraList.front();
            m_freeCameraList.pop_front();
            m_cameraArray[ newRender->Index_ ].Initialize();

            return data;
        }

        Camera &CameraManager::GetCamera(const GfxHND &handle)
        {
            const _RESOURCEHND *newRender = reinterpret_cast<const _RESOURCEHND*>(&handle);

            UAssert(newRender->ID_ == ID_CAMERA, "Attempted to use invalid handle to get camera!");

            return m_cameraArray[ newRender->Index_ ];
        }

        void CameraManager::DestroyCamera(GfxHND& handle)
        {
            _RESOURCEHND *newRender = reinterpret_cast<_RESOURCEHND*>(&handle);

            UAssert(newRender->ID_ == ID_CAMERA, "Attempted to destroy invalid camera handle!");
            m_cameraArray[ newRender->Index_ ].Uninitialize();
            m_freeCameraList.push_front(newRender->Index_);
            handle = 0;
        }
    }
}
