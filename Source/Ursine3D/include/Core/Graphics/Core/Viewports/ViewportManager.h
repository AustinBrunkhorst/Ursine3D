/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      ViewportManager.h
Module:         Graphics
Purpose:        Class for handling multiple viewports
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include <vector>
#include <list>

#include "Viewport.h"
#include "GfxDefines.h"
#include "RenderTargetManager.h"


namespace ursine
{
    namespace graphics
    {
        class ViewportManager
        {
        public:
            void Initialize(DXCore::RenderTargetManager *mgr);
            void Uninitialize(void);

            GfxHND AddViewport(unsigned width, unsigned height);
            void DestroyViewport(GfxHND &viewportHandle);

            Viewport &GetViewport(GfxHND &viewportHandle);

        private:
            std::vector<Viewport> m_viewports;
            std::list<unsigned> m_availableViewports;
            DXCore::RenderTargetManager *m_renderMgr;
        };
    }
}
