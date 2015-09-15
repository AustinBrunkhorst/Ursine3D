#pragma once

#include "GraphicsDefines.h"
#include "ViewportRenderModes.h"

namespace ursine
{
  struct Viewport
  {
    inline void Reset( )
    {
      m_color[ 0 ] = 0;
      m_color[ 1 ] = 0;
      m_color[ 2 ] = 0;
      m_color[ 3 ] = 1;

      m_layer = 0;

      m_size = 0;
      m_height = 0;

      m_xPos = 0;
      m_yPos = 0;

      m_rt = -1;

      m_renderMode = VIEWPORT_RENDER_DEFERRED;

      m_active = false;
    }

    float m_color[ 4 ];       // background color
    unsigned m_layer;         // rendering order

    unsigned m_size;          // dimensions
    unsigned m_height;

    int m_xPos;               // position
    int m_yPos;

    bool m_active;            //is this one in use?

    GFXHND m_camera;          //camera index

    ViewportRenderMode m_renderMode;  //render in forward or deferred mode?

    unsigned m_rt;            //current render target to use
  };
}
