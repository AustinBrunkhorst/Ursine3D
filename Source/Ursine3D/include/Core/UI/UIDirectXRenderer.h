#pragma once

#if defined(URSINE_GRAPHICS_DIRECTX)

#include <cef_render_handler.h>

namespace ursine
{
  class UIDirectXRenderer : public CefRenderHandler
  {
  public:
      UIDirectXRenderer(void);
      virtual ~UIDirectXRenderer(void);

      void Draw(void);

  protected:
      CefRect m_viewport;

  private:
      int m_width, m_height;

      CefRect m_popupBounds;

      bool GetViewRect(CefRefPtr<CefBrowser> browser,
        CefRect &bounds) override;

      void OnPopupShow(CefRefPtr<CefBrowser> browser,
        bool show) override;

      void OnPopupSize(CefRefPtr<CefBrowser> browser,
        const CefRect &bounds) override;

      void OnPaint(CefRefPtr<CefBrowser> browser,
        PaintElementType type, const RectList &regions,
        const void *buffer, int width, int height) override;

      void paintView(CefRefPtr<CefBrowser> browser,
        PaintElementType type, const RectList &regions,
        const void *buffer, int width, int height);

      void paintPopup(CefRefPtr<CefBrowser> browser,
        PaintElementType type, const RectList &regions,
        const void *buffer, int width, int height);

      IMPLEMENT_REFCOUNTING(UIDirectXRenderer);
  };
}

#endif // defined(URSINE_GRAPHICS_DIRECTX)