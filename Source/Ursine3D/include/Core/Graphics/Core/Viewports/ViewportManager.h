#pragma once

#include <vector>
#include <list>

#include "Viewport.h"
#include "GraphicsDefines.h"
#include "RenderTargetManager.h"


namespace rey_oso
{
  class ViewportManager
  {
  public:
    void Initialize( DXCore::RenderTargetManager *mgr );
    void Uninitialize( void );

    GFXHND AddViewport( unsigned width, unsigned height );
    void DestroyViewport( GFXHND &viewportHandle );

    //position
    void SetPosition( GFXHND viewportHandle, const int x, const int y );
    void GetPosition( GFXHND viewportHandle, int &x, int &y );

    //dimension
    void SetDimension( GFXHND viewportHandle, const unsigned w, const unsigned h );
    void GetDimension( GFXHND viewportHandle, unsigned &w, unsigned &h );

    //draw order
    void SetDrawOrder( GFXHND viewportHandle, unsigned order );
    unsigned GetDrawOrder( GFXHND viewportHandle );

    //color
    void SetColor( GFXHND viewportHandle, float r, float g, float b, float a );
    void GetColor( GFXHND viewportHandle, float &r, float &g, float &b, float &a );

    ViewportRenderMode GetRenderMode( GFXHND viewportHandle );
    void SetRenderMode( GFXHND viewportHandle, ViewportRenderMode mode );

    //camera
    void SetCamera( GFXHND handle, GFXHND camHandle );
    GFXHND GetCamera( GFXHND viewportHandle );

    //render target
    RENDER_TARGETS GetRenderTargetID( GFXHND viewportHandle );

  private:
    std::vector<Viewport> m_viewports;
    std::list<unsigned> m_availableViewports;
    DXCore::RenderTargetManager *m_renderMgr;
  };
}