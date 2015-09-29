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
#include "GraphicsDefines.h"

namespace ursine
{
    class UIInstance : CefRenderHandler
    {
    public:
        void Initialize( ID3D11Device *device, ID3D11DeviceContext *context, DXCore::RenderTargetManager *rtmgr, void *mgr, RENDER_TARGETS target );
        void Uninitialize( );

        void Draw( GFXHND viewport = 0);

        void DrawMain( );

        bool GetViewRect( CefRefPtr<CefBrowser> browser,
            CefRect &bounds ) override;

        void OnPopupShow( CefRefPtr<CefBrowser> browser,
            bool show ) override;

        void OnPopupSize( CefRefPtr<CefBrowser> browser,
            const CefRect &bounds ) override;

        void OnPaint( CefRefPtr<CefBrowser> browser,
            PaintElementType type, const RectList &regions,
            const void *buffer, int width, int height ) override;

        void paintView( CefRefPtr<CefBrowser> browser,
            PaintElementType type, const RectList &regions,
            const void *buffer, int width, int height );

        void paintPopup( CefRefPtr<CefBrowser> browser,
            PaintElementType type, const RectList &regions,
            const void *buffer, int width, int height );

        void Resize( int width, int height );

        //private members
    private:
        int m_width, m_height;

        CefRect m_popm_upbounds;

        //device context
        ID3D11Device *m_device;
        ID3D11DeviceContext *m_context;
        void * m_gfxmgr;


        RENDER_TARGETS m_target;
        //rtmanager
        DXCore::RenderTargetManager *m_rtManager;

        IMPLEMENT_REFCOUNTING( UIInstance );
    };
}