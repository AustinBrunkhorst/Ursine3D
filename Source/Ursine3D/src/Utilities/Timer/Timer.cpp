/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Timer.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Timer.h"
#include "TimerManager.h"

namespace ursine
{
    Timer::Timer(const TimeSpan &duration, TimerGroupID group)
        : _group(group)
        , _repeat(0)
        , _deleting(false)
        , _paused(false)
        , _duration(duration)
        , _elapsed(0.0f)
        , _repeated(nullptr)
        , _completed(nullptr)
        , _removed(nullptr)
    {

    }

    Timer::~Timer(void)
    {

    }

    TimerID Timer::Create(const TimeSpan &duration, TimerGroupID group)
    {
        UAssert(duration >= TimeSpan::Zero, 
            "Timer duration must be non-negative.");

        return gTimerManager->create(duration, group);
    }
}
