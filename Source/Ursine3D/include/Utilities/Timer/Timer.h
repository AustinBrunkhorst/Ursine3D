/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Timer.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "TimerConfig.h"
#include "TimerID.h"

#include "TimeSpan.h"

namespace ursine
{
    class Timer
    {
        friend class TimerManager;
        friend class LocalTimerManager;
        friend class TimerID;

        // group id (defaults to zero)
        TimerGroupID _group;

        // number of times to repeat this timer
        int _repeat;

        // determines if the timer is being deleted from the TimerManager
        bool _deleting;

        // determines if the timer will be updated
        bool _paused;

        // duration in milliseconds
        TimeSpan _duration;

        // time elapsed in milliseconds
        TimeSpan _elapsed;

        // called each time the timer repeats
        TimerCallback _repeated;

        // called when the timer has finished repeating
        TimerCallback _completed;

        // called when the timer is removed from the TimerManager.
        // this is only used by LocalTimerManagers
        TimerCallback _removed;

        Timer(const TimeSpan &duration, TimerGroupID group);
    public:
        ~Timer(void);

        // Default timer group
        static const TimerGroupID GROUP_DEFAULT = 0;

        // Value to repeat forever
        static const int REPEAT_FOREVER = -1;

        // Creates a timer from milliseconds
        static TimerID Create(const TimeSpan &duration, TimerGroupID group = GROUP_DEFAULT);
    };
}