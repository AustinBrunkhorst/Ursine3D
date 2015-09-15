#pragma once


#include "GFXAPIDefines.h"

#include "ViewportRenderModes.h"

namespace ursine
{
  class ViewportAPI
  {
    friend class GfxAPI;
  private:
    struct privData;

  public:
    //create a viewport
    GFXHND CreateViewport( unsigned width, unsigned height );

    //destroy a viewport
    void DestroyViewport( GFXHND &handle );

    //screen position (0, 0) is center, sides are half width and half height
    void SetPosition( GFXHND viewportHandle, const int x, const int y );

    //size in pixels... please call sparingly
    void SetDimension( GFXHND viewportHandle, const unsigned w, const unsigned h );

    //where to draw this guy?
    void SetDrawOrder( GFXHND viewportHandle, unsigned order );

    //set background color, NOT IMPLEMENTED YET
    void SetColor( GFXHND viewportHandle, float r, float g, float b, float a );

    //set camera
    void SetCamera( GFXHND handle, GFXHND camHandle );

    void SetRenderMode( GFXHND handle, ViewportRenderMode mode );

  private:
    privData *m_privates;

  private:
    void SetPrivates( void *priv );
    void Initialize( );
    void Uninitialize( );
  };
}