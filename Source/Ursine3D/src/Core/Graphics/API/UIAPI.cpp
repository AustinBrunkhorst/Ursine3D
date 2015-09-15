#include "UrsinePrecompiled.h"
#include "UIAPI.h"

#include "GfxUIManager.h"

namespace ursine
{
  struct UIAPI::privData
  {
    GfxUIManager *mgr;
  };

  bool UIAPI::GetViewRect( CefRefPtr<CefBrowser> browser,
    CefRect &bounds ) 
  {
    return m_privates->mgr->GetViewRect( browser, bounds );
  }

  void UIAPI::OnPopupShow( CefRefPtr<CefBrowser> browser,
    bool show )
  {
    m_privates->mgr->OnPopupShow( browser, show );
  }

  void UIAPI::OnPopupSize( CefRefPtr<CefBrowser> browser,
    const CefRect &bounds )
  {
    m_privates->mgr->OnPopupSize( browser, bounds );
  }

  void UIAPI::OnPaint( CefRefPtr<CefBrowser> browser,
    PaintElementType type, const RectList &regions,
    const void *buffer, int width, int height )
  {
    m_privates->mgr->OnPaint( browser, type, regions, buffer, width, height );
  }

  void UIAPI::paintView( CefRefPtr<CefBrowser> browser,
    PaintElementType type, const RectList &regions,
    const void *buffer, int width, int height )
  {
    m_privates->mgr->paintView( browser, type, regions, buffer, width, height );
  }

  void UIAPI::paintPopup( CefRefPtr<CefBrowser> browser,
    PaintElementType type, const RectList &regions,
    const void *buffer, int width, int height )
  {
    m_privates->mgr->paintPopup( browser, type, regions, buffer, width, height );
  }

  void UIAPI::SetPrivates( void *priv )
  {
    m_privates->mgr = reinterpret_cast<GfxUIManager*>(priv);
  }

  void UIAPI::Initialize( )
  {
    m_privates = new UIAPI::privData( );
  }

  void UIAPI::Uninitialize( )
  {
    delete m_privates;
  }
}