#include "UrsinePrecompiled.h"

#include "DrawingAPI.h"
#include "DrawingManager.h"

namespace rey_oso
{
  struct DrawingAPI::privData
  {
    DrawingManager *mgr;
  };

  void DrawingAPI::DrawPoint( float x, float y, float z )
  {
    m_privates->mgr->DrawPoint( x, y, z );
  }
  void DrawingAPI::DrawLine( float x0, float y0, float z0, float x1, float y1, float z1 )
  {
    m_privates->mgr->DrawLine( x0, y0, z0, x1, y1, z1 );
  }

  void DrawingAPI::SetColor( float r, float g, float b, float a )
  {
    m_privates->mgr->SetDrawColor( r, g, b, a );
  }
  void DrawingAPI::SetSize( float size )
  {
    m_privates->mgr->SetSize( size );
  }

  void DrawingAPI::SetPrivates( void *priv )
  {
    m_privates->mgr = reinterpret_cast<DrawingManager*>(priv);
  }
  void DrawingAPI::Initialize( )
  {
    m_privates = new privData;
  }
  void DrawingAPI::Uninitialize( )
  {
    delete m_privates;
  }
}