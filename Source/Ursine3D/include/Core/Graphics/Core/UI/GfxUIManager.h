/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      GfxUIManager.h
Module:         Graphics
Purpose:        Class for handling the UI
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include "RenderTargetManager.h"
#include "UIInstance.h"

namespace ursine
{
    class GfxUIManager
    {
    public:
        void Initialize(ID3D11Device *device, ID3D11DeviceContext *context, DXCore::RenderTargetManager *rtmgr, void *mgr);
        void Uninitialize();

        //create UI
        GFXHND CreateUI();

        //destroy UI
        void DestroyUI(GFXHND);

        //get UI
        UIInstance &GetUI(GFXHND);

        //private members
    private:

        std::vector<UIInstance*> m_uiInstances;
        std::list<unsigned> m_freeInstances;

        std::list<RENDER_TARGETS> m_freeTargets;

        //device context
        ID3D11Device *m_device;
        ID3D11DeviceContext *m_context;
        DXCore::RenderTargetManager *m_rtManager;
        void *m_gfxmgr;
    };
}
