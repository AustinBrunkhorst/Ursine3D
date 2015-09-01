/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TimerConfig.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <functional>

namespace ursine
{
    typedef std::function<void(void)> TimerCallback;

    typedef int TimerGroupID;

    // arbitrary group ID for timers that are created globally
    const TimerGroupID kTimerGroupGlobal = -4;

    // arbitrary group ID for timers that are created for gameplay
    const TimerGroupID kTimerGroupGameplay = 1;
}