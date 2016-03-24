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

#pragma once

#include "GfxLogging.h"


//gpu defines
#define MAX_CONST_BUFF          14

//gfx defines
#define RELEASE_RESOURCE(resource) \
  if(resource != nullptr)\
    {\
    resource->Release();\
    resource = nullptr;\
    }

// max profiling targets
#define MAX_PROFILER_TARGETS    24

//maximum simultaneous queries
#define MAX_QUERIES             36

//max cameras
#define MAX_CAMERAS             32

//max points/lines
#define MAX_DRAW_OBJ            800019

//max renderables of each type
#define MAX_RENDERABLES         2000

//max draw calls for a single scene
#define MAX_DRAW_CALLS          2048

#define MAX_BONE_COUNT          60

//handle IDs
#define ID_RENDERABLE   170
#define ID_TEXTURE      113
#define ID_MODEL        146
#define ID_ANIMATION    213
#define ID_CAMERA       123
#define ID_VIEWPORT     42
#define ID_UI           87
#define ID_FONT         243

// internal IDs for models
#define INTERNAL_CUBE               0
#define INTERNAL_SPHERE             1
#define INTERNAL_CONE               2
#define INTERNAL_QUAD               3
#define INTERNAL_POINT_INDICES      4

#define INTERNAL_GEOMETRY_COUNT     5

// internal IDs for textured
#define INTERNAL_MISSING_TEX        0
#define INTERNAL_BLANK_TEX          1

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
