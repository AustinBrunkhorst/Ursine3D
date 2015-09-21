#include "UrsinePrecompiled.h"

#include "Viewport.h"

namespace ursine
{
  void Viewport::Initialize ( )
  {
    m_color[ 4 ];              

    m_camera = 0;              

    m_renderMode = VIEWPORT_RENDER_DEFERRED;

    m_width = 800;                 
    m_height = 600;                

    m_positionX = 0;         
    m_positionY = 0;         

    m_rt = 0;                    
  }

  void Viewport::SetBackgroundColor ( float r, float g, float b, float a )
  {
    m_color[ 0 ] = r;
    m_color[ 1 ] = g;
    m_color[ 2 ] = b;
    m_color[ 3 ] = a;
  }

  const float* Viewport::GetBackgroundColor ( )
  {
    return m_color;
  }

  void Viewport::SetViewportCamera ( GFXHND camera )
  {
    const _RESOURCEHND *newRender = reinterpret_cast<_RESOURCEHND*>(&camera);

    UAssert( newRender->ID_ == ID_CAMERA, "Attempted to give invalid handle to viewport!" );

    m_camera = camera;
  }

  GFXHND Viewport::GetViewportCamera ( )
  {
    UAssert( m_camera != 0, "Attempted to get camera from viewport without setting one!" );

    return m_camera;
  }

  void Viewport::SetRenderMode ( ViewportRenderMode renderMode )
  {
    m_renderMode = renderMode;
  }

  ViewportRenderMode Viewport::GetRenderMode ( )
  {
    return m_renderMode;
  }

  void Viewport::SetDimensions ( unsigned width, unsigned height )
  {
    m_width = width;
    m_height = height;
  }

  void Viewport::SetPosition ( unsigned positionX, unsigned positionY )
  {
    m_positionX = positionX;
    m_positionY = positionY;
  }

  D3D11_VIEWPORT Viewport::GetViewportData ( )
  {
    D3D11_VIEWPORT viewport;
    ZeroMemory( &viewport, sizeof( viewport ) );

    viewport.Width = static_cast<float>(m_width);
    viewport.Height = static_cast<float>(m_height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = static_cast<float>(m_positionX);
    viewport.TopLeftY = static_cast<float>(m_positionY);

    return viewport;
  }
}