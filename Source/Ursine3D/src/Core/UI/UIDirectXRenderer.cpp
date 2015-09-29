#include "UrsinePrecompiled.h"

#include "UIDirectXRenderer.h"

namespace ursine
{
    UIDirectXRenderer::UIDirectXRenderer(void)
        : m_width( 0 )
          , m_height( 0 )
    {
        m_gfxMgr = Application::Instance->GetCoreSystem<GfxAPI>( );
    }

    UIDirectXRenderer::~UIDirectXRenderer(void) { }

    bool UIDirectXRenderer::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &bounds)
    {
        bounds = m_viewport;

        return true;
    }

    void UIDirectXRenderer::OnPopupShow(CefRefPtr<CefBrowser> browser, bool show)
    {
        m_gfxMgr->UIMgr.OnPopupShow( browser, show );
    }

    void UIDirectXRenderer::OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect &bounds)
    {
        m_gfxMgr->UIMgr.OnPopupSize( browser, bounds );
    }

    void UIDirectXRenderer::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &regions, const void *buffer, int width, int height)
    {
        m_gfxMgr->UIMgr.OnPaint( browser, type, regions, buffer, width, height );
    }

    void UIDirectXRenderer::paintView(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &regions, const void *buffer, int width, int height)
    {
        m_gfxMgr->UIMgr.paintPopup( browser, type, regions, buffer, width, height );
    }

    void UIDirectXRenderer::paintPopup(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &regions, const void *buffer, int width, int height)
    {
        m_gfxMgr->UIMgr.paintPopup( browser, type, regions, buffer, width, height );
    }
}
