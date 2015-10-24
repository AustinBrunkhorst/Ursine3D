#include "UrsinePrecompiled.h"

#include "ViewportManager.h"
#include "RenderTargetList.h"

namespace ursine
{
    namespace graphics
    {
        void ViewportManager::Initialize(DXCore::RenderTargetManager *mgr)
        {
            m_viewports.resize(RENDER_TARGETS::RENDER_TARGET_MAX_VIEWPORT);

            for (int x = 0; x < RENDER_TARGETS::RENDER_TARGET_MAX_VIEWPORT; ++x)
            {
                m_availableViewports.push_back(x);
            }

            m_renderMgr = mgr;
        }

        void ViewportManager::Uninitialize(void)
        {
            m_renderMgr = nullptr;
        }

        GfxHND ViewportManager::AddViewport(unsigned width, unsigned height)
        {
            UAssert(m_availableViewports.size() > 0, "Out of viewports!");

            GfxHND newHandle = 0;
            _VIEWPORTHND *vpHandle = reinterpret_cast<_VIEWPORTHND*>(&newHandle);


            int x = sizeof(_VIEWPORTHND);
            vpHandle->ID_ = ID_VIEWPORT;

            vpHandle->Index_ = m_availableViewports.front();
            m_availableViewports.pop_front();

            m_viewports[ vpHandle->Index_ ].Initialize();
            m_viewports[ vpHandle->Index_ ].SetDimensions(width, height);

            return newHandle;
        }

        void ViewportManager::DestroyViewport(GfxHND &viewportHandle)
        {
            _VIEWPORTHND *vpHandle = reinterpret_cast<_VIEWPORTHND*>(&viewportHandle);

            UAssert(vpHandle->ID_ == ID_VIEWPORT, "Attempted to use invalid handle to destroy viewport!");

            m_availableViewports.push_front(vpHandle->Index_);

            viewportHandle = 0;
        }

        Viewport &ViewportManager::GetViewport(GfxHND &viewportHandle)
        {
            _VIEWPORTHND *vpHandle = reinterpret_cast<_VIEWPORTHND*>(&viewportHandle);

            UAssert(vpHandle->ID_ == ID_VIEWPORT, "Attempted to use invalid handle to get viewport!");

            return m_viewports[ vpHandle->Index_ ];
        }
    }
}
