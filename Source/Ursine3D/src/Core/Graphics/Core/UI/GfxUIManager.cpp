/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GfxUIManager.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UIManager.h"

#include "GfxManager.h"

#include <d3d11.h>

#define GfxManager reinterpret_cast<GfxManager*>(m_gfxmgr)

namespace ursine
{
    namespace graphics
    {
        void GfxUIManager::Initialize(ID3D11Device *device, ID3D11DeviceContext *context, DXCore::RenderTargetManager *rtmgr, void *gfx)
        {
            m_device = device;
            m_context = context;
            m_gfxmgr = (gfx);

            m_rtManager = rtmgr;

            //DONT FORGET TO CHANGE SIZE
            URSINE_TODO( "Don't forget to update this if UI starts crashing" );
            m_uiInstances.resize(2);

            for (int x = 0; x < 2; ++x)
            {
                m_uiInstances[ x ] = nullptr;
                m_freeInstances.push_back(x);
            }

            m_freeTargets.push_back(RENDER_TARGET_UI);
            m_freeTargets.push_back(RENDER_TARGET_UI2);
            m_freeTargets.push_back(RENDER_TARGET_UI3);
            m_freeTargets.push_back(RENDER_TARGET_UI4);
        }

        void GfxUIManager::Uninitialize()
        {
            for (auto x : m_uiInstances)
            {
                if (x != nullptr)
                {
                    x->Uninitialize();

                    delete x;
                }
            }

            m_device = nullptr;
            m_context = nullptr;
        }

        GfxHND GfxUIManager::CreateUI()
        {
            UAssert(m_freeInstances.size() > 0, "Out of UI instances!");

            GfxHND newHandle;
            _RENDERABLEHND *temp = reinterpret_cast<_RENDERABLEHND*>(&newHandle);

            temp->ID_ = ID_RENDERABLE;
            temp->Index_ = m_freeInstances.front();
            m_freeInstances.pop_front();

            m_uiInstances[ temp->Index_ ] = new UIInstance();

            m_uiInstances[ temp->Index_ ]->Initialize(m_device, m_context, m_rtManager, m_gfxmgr, m_freeTargets.front());
            m_freeTargets.pop_front();

            return newHandle;
        }

        void GfxUIManager::DestroyUI(GfxHND hnd)
        {
            _RENDERABLEHND *handle = reinterpret_cast<_RENDERABLEHND*>(&hnd);

            UAssert(handle->ID_ == ID_RENDERABLE, "Attempted to destroy a UI using an invalid handle!");

            m_uiInstances[ handle->Index_ ]->Uninitialize();

            delete m_uiInstances[ handle->Index_ ];

            m_freeInstances.push_front(handle->Index_);
        }

        UIInstance &GfxUIManager::GetUI(GfxHND hnd)
        {
            _RENDERABLEHND *handle = reinterpret_cast<_RENDERABLEHND*>(&hnd);

            UAssert(handle->ID_ == ID_RENDERABLE, "Attempted to destroy a UI using an invalid handle!");

            return *m_uiInstances[ handle->Index_ ];
        }
    }
}
