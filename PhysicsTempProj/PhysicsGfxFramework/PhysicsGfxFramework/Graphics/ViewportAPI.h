#pragma once

#include "DLLdecl.h"
#include "GFXAPIDefines.h"

#include "ViewportRenderModes.h"

namespace rey_oso
{
  class ViewportAPI
  {
    friend class GfxAPI;
  private:
    struct privData;

  public:
    //create a viewport
    GFX_API GFXHND CreateViewport( unsigned width, unsigned height );

    //destroy a viewport
    GFX_API void DestroyViewport( GFXHND &handle );

    //screen position (0, 0) is center, sides are half width and half height
    GFX_API void SetPosition( GFXHND viewportHandle, const int x, const int y );

    //size in pixels... please call sparingly
    GFX_API void SetDimension( GFXHND viewportHandle, const unsigned w, const unsigned h );

    //where to draw this guy?
    GFX_API void SetDrawOrder( GFXHND viewportHandle, unsigned order );

    //set background color, NOT IMPLEMENTED YET
    GFX_API void SetColor( GFXHND viewportHandle, float r, float g, float b, float a );

    //set camera
    GFX_API void SetCamera( GFXHND handle, GFXHND camHandle );

    //Set the render mode for this viewport
    GFX_API void SetRenderMode( GFXHND handle, ViewportRenderMode mode );

  private:
    privData *m_privates;

  private:
    void SetPrivates( void *priv );
    void Initialize( );
    void Uninitialize( );
  };
}