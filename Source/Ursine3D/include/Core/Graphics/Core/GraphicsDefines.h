/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      GraphicsDefines.h
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
  if(resource != NULL)\
    {\
    resource->Release();\
    resource = NULL;\
    }

//maximum simultaneous queries
#define MAX_QUERIES             36

//max points/lines
#define MAX_DRAW_OBJ            4096

//max renderables of each type
#define MAX_RENDERABLES         2048

//max draw calls for a single scene
#define MAX_DRAW_CALLS 2048

//handle IDs
#define ID_RENDERABLE 170
#define ID_TEXTURE    219
#define ID_MODEL      146
#define ID_CAMERA     123
#define ID_VIEWPORT   42

typedef unsigned long long GFXHND;

#include "GfxHandle.h"