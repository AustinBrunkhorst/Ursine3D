/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      GfxAPI.h
Module:         Graphics
Purpose:        Interface for the bulk of graphics
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/
#pragma once

#include "DLLdecl.h"

#include "GfxConfig.h"
#include "GFXAPIDefines.h"
#include "ResourceAPI.h"
#include "RenderableAPI.h"
#include "CameraAPI.h"
#include "ViewportAPI.h"
#include "UIAPI.h"
#include "DrawingAPI.h"

namespace rey_oso
{
  class GfxAPI
  {
  private:
    struct privateData;

    //public members
  public:
    //manage resources like textures, models
    //RenderableManager
    ResourceAPI ResourceMgr;
    RenderableAPI RenderableMgr;
    CameraAPI CameraMgr;
    ViewportAPI ViewportMgr;
    UIAPI UIMgr;
    DrawingAPI DrawingMgr;

    //public methods
  public:
    // create the gfx interface, requires config object
    static GFX_API GfxAPI *CreateGfxInterface( GfxConfig &config ); 

     // destroy everything
    static GFX_API void DestroyGfxInterface( ); 

    // render an object                                                                   
    GFX_API void RenderObject( GFXHND handle );      

    // begin to render a scene                                                                   
    GFX_API void BeginScene( );   

    // render the scene
    GFX_API void RenderScene(float dt, GFXHND viewport );   

    // end the current scene
    GFX_API void EndScene( );                                       

    // private members
  private:
    // internal pointer
    static GfxAPI *m_interface;
    privateData *m_privates;

    // private methods
  private:
    // no touchy
    GfxAPI( GfxConfig &config );
    GfxAPI( );
    ~GfxAPI( );
  };
}