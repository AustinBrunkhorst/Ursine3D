#pragma once



#include "GFXAPIDefines.h"

namespace ursine
{
  class DrawingAPI
  {
    friend class GfxAPI;
  private:
    struct privData;

  public:
    // draw a point
    void DrawPoint( float x, float y, float z );      
    // draw a line 
    void DrawLine( float x0, float y0, float z0, float x1, float y1, float z1 );  
                                       
    // set render color
    void SetColor( float r, float g, float b, float a );        

    // size of the points, in pixels
    void SetSize( float size );                                                   
  private:
    privData *m_privates;

  private:
    void SetPrivates( void *priv );
    void Initialize( );
    void Uninitialize( );
  };
}