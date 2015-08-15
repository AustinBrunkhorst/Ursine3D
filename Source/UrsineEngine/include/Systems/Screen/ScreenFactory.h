/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ScreenFactory.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Screen.h"

namespace Ursine
{
    template<typename ScreenType>
    Screen *ScreenFactory(void);
}

#include "ScreenFactory.hpp"