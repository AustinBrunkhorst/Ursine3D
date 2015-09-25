#pragma once

#include <vector>
#include <list>

#include "Viewport.h"
#include "GraphicsDefines.h"
#include "RenderTargetManager.h"


namespace ursine
{
  class ViewportManager
  {
  public:
    void Initialize( DXCore::RenderTargetManager *mgr );
    void Uninitialize( void );

    GFXHND AddViewport( unsigned width, unsigned height );
    void DestroyViewport( GFXHND &viewportHandle );

    Viewport &GetViewport( GFXHND &viewportHandle );

  private:
    std::vector<Viewport> m_viewports;
    std::list<unsigned> m_availableViewports;
    DXCore::RenderTargetManager *m_renderMgr;
  };
}