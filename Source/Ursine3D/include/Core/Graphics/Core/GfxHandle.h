/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      GfxHandle.h
Module:         Graphics
Purpose:        Main handle class, for use in the core graphics engine
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

typedef unsigned long long GFXHND;

//converting to types
#define HND_DRAW(handle)    reinterpret_cast<_DRAWHND*>(&handle)
#define HND_RSRCE(handle)   reinterpret_cast<_RESOURCEHND*>(&handle)
#define HND_RENDER(handle)  reinterpret_cast<_RENDERABLEHND*>(&handle)
#define HND_CAST(handle)    static_cast<GFXHND>(handle)

namespace ursine
{
    //internal structure for draw calls
    struct _DRAWHND
    {
        unsigned Index_ : 16;
        unsigned Type_ : 4;
        unsigned Material_ : 8; //what material do I have?
        unsigned Model_ : 8; //what model do we have
        unsigned Shader_ : 8; //what shader am I?
        unsigned HUD_ : 1; //are we a part of the hud?
        unsigned buffer_ : 7; //buffer for packing
        unsigned debug_ : 4; //debug happens last
        unsigned ID_ : 8; //sanity check ID
    };

    //internal structure for resources
    struct _RESOURCEHND
    {
        unsigned Index_ : 16; //index of resource
        unsigned Etc_ : 16; //extra data... what type of texture? maybe what model it is associated with?
        unsigned Type_ : 4; //what is this? a model? texture? 
        unsigned buffer_ : 20; //buffer for packing
        unsigned ID_ : 8; //sanity check ID
    };

    //internal structure for renderables given to user
    struct _RENDERABLEHND
    {
        unsigned Index_ : 16; //actual data
        unsigned Type_ : 8; //what type is this resource? a 3d model? a light? 2d billboard?
        unsigned ID_ : 8; //sanity check ID
        unsigned buffer_ : 32; //buffer
    };

    //internal structure for renderables given to user
    struct _VIEWPORTHND
    {
        unsigned Index_ : 16; //actual data
        unsigned buffer1_ : 16; //buffer
        unsigned buffer2_ : 24; //buffer
        unsigned ID_ : 8; //sanity check ID
    };
}
