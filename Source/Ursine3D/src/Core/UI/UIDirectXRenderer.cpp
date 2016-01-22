/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UIDirectXRenderer.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UIDirectXRenderer.h"

namespace ursine
{
    UIDirectXRenderer::UIDirectXRenderer(void)
        : m_gfxMgr( GetCoreSystem( graphics::GfxAPI ) )
        , m_uiHandle( m_gfxMgr->UIMgr.CreateUI( ) )
    {

    }

    UIDirectXRenderer::~UIDirectXRenderer(void)
    {
        //@UI
        //m_gfxMgr->UIMgr.DestroyUI( m_uiHandle );
        //m_uiHandle = 0;
    }

    void UIDirectXRenderer::Draw(GfxHND camera)
    {
        m_gfxMgr->UIMgr.GetUI( m_uiHandle ).Draw( camera );
    }

    void UIDirectXRenderer::DrawMain(void)
    {
        m_gfxMgr->UIMgr.GetUI( m_uiHandle ).DrawMain( );
    }

    void UIDirectXRenderer::resize(const int width, const int height)
    {
        m_gfxMgr->UIMgr.GetUI( m_uiHandle ).Resize( width, height );
    }

    bool UIDirectXRenderer::GetViewRect(
        CefRefPtr<CefBrowser> browser, 
        CefRect &bounds
    )
    {
        return m_gfxMgr->UIMgr
            .GetUI( m_uiHandle ).GetViewRect( browser, bounds );
    }

    void UIDirectXRenderer::OnPopupShow(
        CefRefPtr<CefBrowser> browser, 
        bool show
    )
    {
        m_gfxMgr->UIMgr
            .GetUI( m_uiHandle ).OnPopupShow( browser, show );
    }

    void UIDirectXRenderer::OnPopupSize(
        CefRefPtr<CefBrowser> browser, 
        const CefRect &bounds
    )
    {
        m_gfxMgr->UIMgr
            .GetUI( m_uiHandle ).OnPopupSize( browser, bounds );
    }

    void UIDirectXRenderer::OnPaint(
        CefRefPtr<CefBrowser> browser, 
        PaintElementType type, 
        const RectList &regions, 
        const void *buffer, 
        int width, 
        int height
    )
    {
        m_gfxMgr->UIMgr.GetUI( m_uiHandle )
            .OnPaint( browser, type, regions, buffer, width, height );
    }
}
