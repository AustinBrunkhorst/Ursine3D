#include "UrsinePrecompiled.h"

#if defined(URSINE_GRAPHICS_DIRECTX)

#include "UIDirectXRenderer.h"

#include "GfxAPI.h"

namespace ursine
{
  UIDirectXRenderer::UIDirectXRenderer(void)
      : m_width( 0 )
      , m_height( 0 )
  {

  }

  UIDirectXRenderer::~UIDirectXRenderer(void)
  {

  }

  void UIDirectXRenderer::Draw(void)
  {

  }

  bool UIDirectXRenderer::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &bounds)
  {
      bounds = m_viewport;

      return true;
  }

  void UIDirectXRenderer::OnPopupShow(CefRefPtr<CefBrowser> browser, bool show)
  {
    auto *gfxManager = Application::Instance->GetCoreSystem<ursine::GfxAPI>( );

    gfxManager->UIMgr.OnPopupShow( browser, show );
  }

  void UIDirectXRenderer::OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect &bounds)
  {
    auto *gfxManager = Application::Instance->GetCoreSystem<ursine::GfxAPI>( );

    gfxManager->UIMgr.OnPopupSize( browser, bounds );
  }

  void UIDirectXRenderer::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& regions, const void *buffer, int width, int height)
  {
    auto *gfxManager = Application::Instance->GetCoreSystem<ursine::GfxAPI>( );

    gfxManager->UIMgr.OnPaint( browser, type, regions, buffer, width, height );
  }

  void UIDirectXRenderer::paintView(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& regions, const void *buffer, int width, int height)
  {
    auto *gfxManager = Application::Instance->GetCoreSystem<ursine::GfxAPI>( );

    gfxManager->UIMgr.paintPopup( browser, type, regions, buffer, width, height );
  }

  void UIDirectXRenderer::paintPopup(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &regions, const void *buffer, int width, int height)
  {
    auto *gfxManager = Application::Instance->GetCoreSystem<ursine::GfxAPI>( );

    gfxManager->UIMgr.paintPopup( browser, type, regions, buffer, width, height );
  }
}

#endif // defined(URSINE_GRAPHICS_DIRECTX)