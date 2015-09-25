#pragma once



#include <cef_render_handler.h>

#include "GfxAPIDefines.h"

namespace ursine
{
  class UIAPI : public CefRenderHandler
  {
    friend class GfxAPI;
  private:
    struct privData;

  public:
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

  private:
    privData *m_privates;

  private:
    void SetPrivates( void *priv );
    void Initialize( );
    void Uninitialize( );

    IMPLEMENT_REFCOUNTING( UIAPI );
  };
}