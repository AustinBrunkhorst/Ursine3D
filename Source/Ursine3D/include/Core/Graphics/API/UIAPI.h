#pragma once



#include <cef_render_handler.h>

#include "GfxAPIDefines.h"
#include "UIInstance.h"

namespace ursine
{
  class UIAPI
  {
    friend class GfxAPI;
  private:
    struct privData;

  public:
      //create a viewport
      GFXHND CreateUI( );

      //get ui
      UIInstance& GetUI( GFXHND UI );

      //destroy a viewport
      void DestroyUI( GFXHND  UI );

  private:
    privData *m_privates;

  private:
    void SetPrivates( void *priv );
    void Initialize( );
    void Uninitialize( );
  };
}