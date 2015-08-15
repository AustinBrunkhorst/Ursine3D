/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ScreenConfig.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "ScreenState.h"
#include "Json.h"

#include "UrsineTypes.h"

namespace Ursine
{
    class Screen;

    typedef uint32 ScreenID;
    typedef Screen*(*ScreenFactoryCallback)(void);
}