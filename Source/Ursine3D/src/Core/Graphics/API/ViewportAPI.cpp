#include "UrsinePrecompiled.h"

#include "ViewportAPI.h"
#include "ViewportManager.h"

namespace ursine
{
  struct ViewportAPI::privData
  {
    ViewportManager *mgr;
  };

  GFXHND ViewportAPI::CreateViewport( unsigned width, unsigned height )
  {
    return m_privates->mgr->AddViewport( width, height );
  }

  void ViewportAPI::DestroyViewport( GFXHND &handle )
  {
    m_privates->mgr->DestroyViewport( handle );
  }

  void ViewportAPI::SetPosition( GFXHND viewportHandle, const int x, const int y )
  {
    m_privates->mgr->SetPosition( viewportHandle, x, y );
  }
  void ViewportAPI::SetDimension( GFXHND viewportHandle, const unsigned w, const unsigned h )
  {
    m_privates->mgr->SetDimension( viewportHandle, w, h );
  }
  void ViewportAPI::SetDrawOrder( GFXHND viewportHandle, unsigned order )
  {
    m_privates->mgr->SetDrawOrder( viewportHandle, order );
  }
  void ViewportAPI::SetColor( GFXHND viewportHandle, float r, float g, float b, float a )
  {
    m_privates->mgr->SetColor( viewportHandle, r, g, b, a );
  }
  void ViewportAPI::SetCamera( GFXHND handle, GFXHND camHandle )
  {
    m_privates->mgr->SetCamera( handle, camHandle );
  }
  void ViewportAPI::SetRenderMode( GFXHND handle, ViewportRenderMode mode )
  {
    m_privates->mgr->SetRenderMode( handle, mode );
  }

  void ViewportAPI::SetPrivates( void *priv)
  {
    m_privates->mgr = reinterpret_cast<ViewportManager*>(priv);
  }
  void ViewportAPI::Initialize( )
  {
    m_privates = new privData;
  }
  void ViewportAPI::Uninitialize( )
  {

  }
}