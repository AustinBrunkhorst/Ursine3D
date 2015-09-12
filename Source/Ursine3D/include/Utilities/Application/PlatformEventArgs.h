/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** InternalApplicationEvent.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <SDL.h>

#include "EventArgs.h"

namespace ursine
{
    struct PlatformEventArgs : public EventArgs
    {
        SDL_Event *data;
    };
}