/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MouseButton.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "SDL_mouse.h"

namespace ursine 
{
    enum MouseButton
    {
        MBTN_LEFT = SDL_BUTTON_LEFT,
        MBTN_MIDDLE = SDL_BUTTON_MIDDLE,
        MBTN_RIGHT = SDL_BUTTON_RIGHT,
        MBTN_COUNT
    };
}