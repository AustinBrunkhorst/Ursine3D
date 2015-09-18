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

  Viewport& ViewportAPI::GetViewport ( GFXHND& handle )
  {
    return m_privates->mgr->GetViewport( handle );
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