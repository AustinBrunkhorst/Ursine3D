#include "UrsinePrecompiled.h"

#include "UIDirectXRenderer.h"

namespace ursine
{
    UIDirectXRenderer::UIDirectXRenderer(void)
        : m_width( 0 )
        , m_height( 0 )
    {
        m_gfxMgr = CoreSystem( GfxAPI );
        m_uiHandle = m_gfxMgr->UIMgr.CreateUI( );
    }

    UIDirectXRenderer::~UIDirectXRenderer(void)
    {
        //@UI
        //m_gfxMgr->UIMgr.DestroyUI( m_uiHandle );
        //m_uiHandle = 0;
    }

    void UIDirectXRenderer::Draw(GFXHND camera)
    {
        m_gfxMgr->UIMgr.GetUI( m_uiHandle ).Draw( camera );
    }

    void UIDirectXRenderer::DrawMain(void)
    {
        m_gfxMgr->UIMgr.GetUI( m_uiHandle ).DrawMain( );
    }

    void UIDirectXRenderer::resize(const int width, const int height)
    {
        m_gfxMgr->UIMgr.GetUI(m_uiHandle).Resize(width, height);
    }

    bool UIDirectXRenderer::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &bounds)
    {
        bounds = m_viewport;

        return true;
    }

    void UIDirectXRenderer::OnPopupShow(CefRefPtr<CefBrowser> browser, bool show)
    {
        m_gfxMgr->UIMgr.GetUI( m_uiHandle ).OnPopupShow( browser, show );
    }

    void UIDirectXRenderer::OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect &bounds)
    {
        m_gfxMgr->UIMgr.GetUI( m_uiHandle ).OnPopupSize( browser, bounds );
    }

    void UIDirectXRenderer::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &regions, const void *buffer, int width, int height)
    {
        m_gfxMgr->UIMgr.GetUI( m_uiHandle ).OnPaint( browser, type, regions, buffer, width, height );
    }

    void UIDirectXRenderer::paintView(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &regions, const void *buffer, int width, int height)
    {
        //@UI
        m_gfxMgr->UIMgr.GetUI( m_uiHandle ).paintPopup( browser, type, regions, buffer, width, height );
    }

    void UIDirectXRenderer::paintPopup(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &regions, const void *buffer, int width, int height)
    {
        //@UI
        m_gfxMgr->UIMgr.GetUI( m_uiHandle ).paintPopup( browser, type, regions, buffer, width, height );
    }
}
