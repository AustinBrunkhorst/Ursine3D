#pragma once

#include <d3d11.h>
#include <cef_render_handler.h>
#include "RenderTargetManager.h"
#include "GraphicsDefines.h"

namespace ursine
{
  class GfxUIManager : CefRenderHandler
  {
  public:
    void Initialize( ID3D11Device *device, ID3D11DeviceContext *context, DXCore::RenderTargetManager *rtmgr, void *mgr);
    void Uninitialize( );

    //private methods
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

    void Resize( int width, int height );

    //private members
  private:
    int m_width, m_height;

    CefRect m_popm_upbounds;

    //device context
    ID3D11Device *m_device;
    ID3D11DeviceContext *m_context;
    void * m_gfxmgr;

    //rtmanager
    DXCore::RenderTargetManager *m_rtManager;

    IMPLEMENT_REFCOUNTING( GfxUIManager );
  };
}