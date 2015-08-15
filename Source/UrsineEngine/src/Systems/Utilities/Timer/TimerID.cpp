/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TimerID.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "TimerID.h"
#include "TimerManager.h"

namespace Ursine
{
    // overflow will make this large, and thus invalid (will never conflict
    // with a real ID)
    TimerID::TimerID(void)
        : _id(-1) {}

    TimerID::TimerID(uint32 id)
        : _id(id) {}

    bool TimerID::IsActive(void)
    {
        auto timer = gTimerManager->get(_id);

        return timer && !timer->_deleting;
    }

    bool TimerID::IsPaused(void) const
    {
        auto timer = gTimerManager->get(_id);

        if (!timer)
            return true;

        return timer->_paused || gTimerManager->_groups[timer->_group];
    }

    void TimerID::Pause(void)
    {
        auto timer = gTimerManager->get(_id);

        if (timer)
            timer->_paused = true;
    }

    void TimerID::Resume(void)
    {
        auto timer = gTimerManager->get(_id);

        if (timer)
            timer->_paused = false;
    }

    void TimerID::Cancel(void) const
    {
        gTimerManager->cancel(_id);
    }

    TimerID &TimerID::Repeat(int count)
    {
        UAssert(count == Timer::REPEAT_FOREVER || count >= 0,
            "Timer repeation count must be non-negative.");

        auto timer = gTimerManager->get(_id);

        if (timer)
            timer->_repeat = count;

        return *this;
    }

    TimerID &TimerID::Repeated(TimerCallback callback)
    {
        auto timer = gTimerManager->get(_id);

        if (timer)
            timer->_repeated = callback;

        return *this;
    }

    TimerID &TimerID::Completed(TimerCallback callback)
    {
        auto timer = gTimerManager->get(_id);

        if (timer)
            timer->_completed = callback;

        return *this;
    }

    const TimeSpan &TimerID::GetDuration(void) const
    {
        auto timer = gTimerManager->get(_id);

        return timer ? timer->_duration : TimeSpan::Zero;
    }

    const TimeSpan &TimerID::GetElapsed(void) const
    {
        auto timer = gTimerManager->get(_id);

        return timer ? timer->_elapsed : TimeSpan::Zero;
    }

    TimerID &TimerID::Removed(TimerCallback callback)
    {
        auto timer = gTimerManager->get(_id);

        if (timer)
            timer->_removed = callback;

        return *this;
    }
}