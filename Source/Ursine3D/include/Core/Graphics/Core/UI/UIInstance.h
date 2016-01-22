/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UIInstance.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      UIInstance.h
Module:         Graphics
Purpose:        Class for an instance of a UI
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include <d3d11.h>
#include <cef_render_handler.h>
#include "RenderTargetManager.h"
#include "GfxDefines.h"

namespace ursine
{
    namespace graphics
    {
        class UIInstance
        {
        public:
            void Initialize(
                ID3D11Device *device, 
                ID3D11DeviceContext *context, 
                DXCore::RenderTargetManager *rtmgr, 
                void *mgr, 
                RENDER_TARGETS target
            );

            void Uninitialize(void);

            void Draw(GfxHND camera = 0);

            void DrawMain(void);

            bool GetViewRect(
                CefRefPtr<CefBrowser> browser,
                CefRect &bounds
            );

            void OnPopupShow(
                CefRefPtr<CefBrowser> browser,
                bool show
            );

            void OnPopupSize(
                CefRefPtr<CefBrowser> browser,
                const CefRect &bounds
            );

            void OnPaint(
                CefRefPtr<CefBrowser> browser,
                CefRenderHandler::PaintElementType type, 
                const CefRenderHandler::RectList &regions,
                const void *buffer, 
                int width, 
                int height
            );

            void Resize(int width, int height);

            //private members
        private:
            int m_width, m_height;

            bool m_paintingPopup;

            CefRect m_popupRect;
            CefRect m_originalPopupRect;

            //device context
            ID3D11Device *m_device;
            ID3D11DeviceContext *m_context;
            void *m_gfxmgr;

            GfxHND m_mainTarget;
            GfxHND m_popup;

            CefRect getPopupRectInView(const CefRect &original);

            void paintView(CefRefPtr<CefBrowser> browser,
                CefRenderHandler::PaintElementType type, 
                const CefRenderHandler::RectList &regions,
                const void *buffer, 
                int width, 
                int height
            );

            void paintPopup(
                CefRefPtr<CefBrowser> browser,
                CefRenderHandler::PaintElementType type, 
                const CefRenderHandler::RectList &regions,
                const void *buffer, 
                int width, 
                int height
            );
        };
    }
}
