/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      ResourceAPI.h
Module:         Graphics
Purpose:        interface for handling resources
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once
#include "DLLdecl.h"
#include "GFXAPIDefines.h"

namespace rey_oso
{
  class ResourceAPI
  {
    friend class GfxAPI;
  private:
    struct privData;

  public:
    // get model handle
    GFX_API GFXHND GetModelHandle( const char *name );

    // get texture handle
    GFX_API GFXHND GetTexHandle( const char *name );

  private:
    privData *m_privates;

  private:
    GFX_API void SetPrivates( void *priv, void *priv2 );
    GFX_API void Initialize( );
    GFX_API void Uninitialize( );
  };
}