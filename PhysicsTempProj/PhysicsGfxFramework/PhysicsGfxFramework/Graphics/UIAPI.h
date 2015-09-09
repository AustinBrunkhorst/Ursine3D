/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      UIAPI.h
Module:         Graphics
Purpose:        Camera class for viewing 3d world
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include "DLLdecl.h"

//#include <cef_render_handler.h>

#include "GfxAPIDefines.h"

//this is a hack please don't try to use UI
class CefRenderHandler
{

};

template<typename T>
class CefRefPtr
{

};

class CefBrowser
{

};

class PaintElementType
{

};

class CefRect
{

};

class RectList
{

};

namespace rey_oso
{
  class UIAPI : public CefRenderHandler
  {
    friend class GfxAPI;
  private:
    struct privData;

  public:
    GFX_API bool GetViewRect( CefRefPtr<CefBrowser> browser,
      CefRect &bounds );

    GFX_API void OnPopupShow( CefRefPtr<CefBrowser> browser,
      bool show );

    GFX_API void OnPopupSize( CefRefPtr<CefBrowser> browser,
      const CefRect &bounds );

    GFX_API void OnPaint( CefRefPtr<CefBrowser> browser,
      PaintElementType type, const RectList &regions,
      const void *buffer, int width, int height );

    GFX_API void paintView( CefRefPtr<CefBrowser> browser,
      PaintElementType type, const RectList &regions,
      const void *buffer, int width, int height );

    GFX_API void paintPopup( CefRefPtr<CefBrowser> browser,
      PaintElementType type, const RectList &regions,
      const void *buffer, int width, int height );

  private:
    privData *m_privates;

  private:
    GFX_API void SetPrivates( void *priv );
    GFX_API void Initialize( );
    GFX_API void Uninitialize( );

    //IMPLEMENT_REFCOUNTING( UIAPI );
  };
}