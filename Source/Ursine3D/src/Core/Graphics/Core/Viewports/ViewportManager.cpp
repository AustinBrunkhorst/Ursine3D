#include "UrsinePrecompiled.h"

#include "ViewportManager.h"
#include "RenderTargetList.h"

namespace rey_oso
{
  void ViewportManager::Initialize( DXCore::RenderTargetManager *mgr )
  {
    m_viewports.resize( RENDER_TARGETS::RENDER_TARGET_MAX_VIEWPORT );

    for (int x = 0; x < RENDER_TARGETS::RENDER_TARGET_MAX_VIEWPORT; ++x)
    {
      m_availableViewports.push_back( x );
      m_viewports[ x ].m_active = false;
    }

    m_renderMgr = mgr;
  }
  void ViewportManager::Uninitialize( void )
  {

  }

  GFXHND ViewportManager::AddViewport( unsigned width, unsigned height )
  {
    UAssert( m_availableViewports.size( ) > 0, "Out of viewports!" );

    GFXHND newHandle = 0;
    _VIEWPORTHND *vpHandle = reinterpret_cast<_VIEWPORTHND*>(&newHandle);


    int x = sizeof( _VIEWPORTHND );
    vpHandle->ID_ = ID_VIEWPORT;

    vpHandle->Index_ = m_availableViewports.front( );
    m_availableViewports.pop_front( );

    m_viewports[ vpHandle->Index_ ].Reset( );
    m_viewports[ vpHandle->Index_ ].m_active = true;
    m_viewports[ vpHandle->Index_ ].m_rt = m_renderMgr->CreateRT( width, height );
    m_viewports[ vpHandle->Index_ ].m_size = width;
    m_viewports[ vpHandle->Index_ ].m_height = height;

    return newHandle;
  }

  void ViewportManager::DestroyViewport( GFXHND &viewportHandle )
  {
    _VIEWPORTHND *vpHandle = reinterpret_cast<_VIEWPORTHND*>(&viewportHandle);

    UAssert( vpHandle->ID_ == ID_VIEWPORT, "Attempted to use invalid handle to get viewport!" );

    m_renderMgr->DestroyRT( (RENDER_TARGETS)m_viewports[ vpHandle->Index_ ].m_rt );

    m_availableViewports.push_front( vpHandle->Index_ );

    m_viewports[ vpHandle->Index_ ].Reset( );

    viewportHandle = 0;
  }

  //position
  void ViewportManager::SetPosition( GFXHND viewportHandle, const int x, const int y )
  {
    _VIEWPORTHND *vpHandle = reinterpret_cast<_VIEWPORTHND*>(&viewportHandle);

    UAssert( vpHandle->ID_ == ID_VIEWPORT, "Attempted to use invalid handle to get viewport!" );

    m_viewports[ vpHandle->Index_ ].m_xPos = x;
    m_viewports[ vpHandle->Index_ ].m_yPos = y;
  }
  void ViewportManager::GetPosition( GFXHND viewportHandle, int &x, int &y )
  {
    _VIEWPORTHND *vpHandle = reinterpret_cast<_VIEWPORTHND*>(&viewportHandle);

    UAssert( vpHandle->ID_ == ID_VIEWPORT, "Attempted to use invalid handle to get viewport!" );

    x = m_viewports[ vpHandle->Index_ ].m_xPos;
    y = m_viewports[ vpHandle->Index_ ].m_yPos;
  }

  //dimension
  void ViewportManager::SetDimension( GFXHND viewportHandle, const unsigned w, const unsigned h )
  {
    _VIEWPORTHND *vpHandle = reinterpret_cast<_VIEWPORTHND*>(&viewportHandle);

    UAssert( vpHandle->ID_ == ID_VIEWPORT, "Attempted to use invalid handle to get viewport!" );

    m_viewports[ vpHandle->Index_ ].m_size = w;
    m_viewports[ vpHandle->Index_ ].m_height = h;
  }
  void ViewportManager::GetDimension( GFXHND viewportHandle, unsigned &w, unsigned &h )
  {
    _VIEWPORTHND *vpHandle = reinterpret_cast<_VIEWPORTHND*>(&viewportHandle);

    UAssert( vpHandle->ID_ == ID_VIEWPORT, "Attempted to use invalid handle to get viewport!" );

    w = m_viewports[ vpHandle->Index_ ].m_size;
    h = m_viewports[ vpHandle->Index_ ].m_height;
  }

  //draw order
  void ViewportManager::SetDrawOrder( GFXHND viewportHandle, unsigned order )
  {
    _VIEWPORTHND *vpHandle = reinterpret_cast<_VIEWPORTHND*>(&viewportHandle);

    UAssert( vpHandle->ID_ == ID_VIEWPORT, "Attempted to use invalid handle to get viewport!" );

    m_viewports[ vpHandle->Index_ ].m_layer = order;
  }
  unsigned ViewportManager::GetDrawOrder( GFXHND viewportHandle )
  {
    _VIEWPORTHND *vpHandle = reinterpret_cast<_VIEWPORTHND*>(&viewportHandle);

    UAssert( vpHandle->ID_ == ID_VIEWPORT, "Attempted to use invalid handle to get viewport!" );

    return m_viewports[ vpHandle->Index_ ].m_layer;
  }

  //color
  void ViewportManager::SetColor( GFXHND viewportHandle, float r, float g, float b, float a )
  {
    _VIEWPORTHND *vpHandle = reinterpret_cast<_VIEWPORTHND*>(&viewportHandle);

    UAssert( vpHandle->ID_ == ID_VIEWPORT, "Attempted to use invalid handle to get viewport!" );

    m_viewports[ vpHandle->Index_ ].m_color[ 0 ] = r;
    m_viewports[ vpHandle->Index_ ].m_color[ 1 ] = g;
    m_viewports[ vpHandle->Index_ ].m_color[ 2 ] = b;
    m_viewports[ vpHandle->Index_ ].m_color[ 3 ] = a;
  }
  void ViewportManager::GetColor( GFXHND viewportHandle, float &r, float &g, float &b, float &a )
  {
    _VIEWPORTHND *vpHandle = reinterpret_cast<_VIEWPORTHND*>(&viewportHandle);

    UAssert( vpHandle->ID_ == ID_VIEWPORT, "Attempted to use invalid handle to get viewport!" );

    r = m_viewports[ vpHandle->Index_ ].m_color[ 0 ];
    g = m_viewports[ vpHandle->Index_ ].m_color[ 1 ];
    b = m_viewports[ vpHandle->Index_ ].m_color[ 2 ];
    a = m_viewports[ vpHandle->Index_ ].m_color[ 3 ];
  }

  ViewportRenderMode ViewportManager::GetRenderMode( GFXHND viewportHandle )
  {
    _VIEWPORTHND *vpHandle = reinterpret_cast<_VIEWPORTHND*>(&viewportHandle);

    UAssert( vpHandle->ID_ == ID_VIEWPORT, "Attempted to use invalid handle to get viewport!" );

    return m_viewports[ vpHandle->Index_ ].m_renderMode;
  }

  void ViewportManager::SetRenderMode( GFXHND viewportHandle, ViewportRenderMode mode )
  {
    _VIEWPORTHND *vpHandle = reinterpret_cast<_VIEWPORTHND*>(&viewportHandle);

    UAssert( vpHandle->ID_ == ID_VIEWPORT, "Attempted to use invalid handle to get viewport!" );

    m_viewports[ vpHandle->Index_ ].m_renderMode = mode;
  }

  //camera
  void ViewportManager::SetCamera( GFXHND viewportHandle, GFXHND camHandle )
  {
    _VIEWPORTHND *vpHandle = reinterpret_cast<_VIEWPORTHND*>(&viewportHandle);

    UAssert( vpHandle->ID_ == ID_VIEWPORT, "Attempted to use invalid handle to get viewport!" );

    _RESOURCEHND *camera = reinterpret_cast<_RESOURCEHND*>(&camHandle);

    UAssert( camera->ID_ == ID_CAMERA, "Attempted to give viewport invalid camera handle!" );

    m_viewports[ vpHandle->Index_ ].m_camera = camHandle;
  }
  GFXHND ViewportManager::GetCamera( GFXHND viewportHandle )
  {
    _VIEWPORTHND *vpHandle = reinterpret_cast<_VIEWPORTHND*>(&viewportHandle);

    UAssert( vpHandle->ID_ == ID_VIEWPORT, "Attempted to use invalid handle to get viewport!" );

    return m_viewports[ vpHandle->Index_ ].m_camera;
  }

  RENDER_TARGETS  ViewportManager::GetRenderTargetID( GFXHND viewportHandle )
  {
    _VIEWPORTHND *vpHandle = reinterpret_cast<_VIEWPORTHND*>(&viewportHandle);

    UAssert( vpHandle->ID_ == ID_VIEWPORT, "Attempted to use invalid handle to get viewport!" );

    return (RENDER_TARGETS)m_viewports[ vpHandle->Index_ ].m_rt;
  }
}