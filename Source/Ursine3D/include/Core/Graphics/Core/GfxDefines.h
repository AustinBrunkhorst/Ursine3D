/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GfxDefines.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      GfxDefines.h
Module:         Graphics
Purpose:        Gfx global defines
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include "GfxLogging.h"


//gpu defines
#define MAX_CONST_BUFF          15

//gfx defines
#define RELEASE_RESOURCE(resource) \
  if(resource != nullptr)\
    {\
    resource->Release();\
    resource = nullptr;\
    }

//maximum simultaneous queries
#define MAX_QUERIES             36

//max cameras
#define MAX_CAMERAS             32

//max points/lines
#define MAX_DRAW_OBJ            80019

//max renderables of each type
#define MAX_RENDERABLES         512

//max draw calls for a single scene
#define MAX_DRAW_CALLS          2048

#define MAX_BONE_COUNT          60

//handle IDs
#define ID_RENDERABLE 170
#define ID_TEXTURE    113
#define ID_MODEL      146
#define ID_CAMERA     123
#define ID_VIEWPORT   42
#define ID_UI         87

//sanity values
#define SANITY_RESOURCE 73

namespace ursine
{
    namespace graphics
    {
        typedef unsigned long long GfxHND;
    }
}

#include "GfxHandle.h"
