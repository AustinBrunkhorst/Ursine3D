/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TimerManager.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "CoreSystem.h"

#include "Timer.h"

namespace ursine
{
    class TimerManager : public core::CoreSystem
    {
        CORE_SYSTEM
    public:
        TimerManager(void);
        ~TimerManager(void);

        // Suspends updating a specific group
        void Pause(TimerGroupID group);

        // Resumes updating a specific group
        void Resume(TimerGroupID group);

        // Removes all timers in a specific group
        void Clear(TimerGroupID group = Timer::GROUP_DEFAULT);

    private:
        friend class Timer;
        friend class TimerID;

        friend class LocalTimerManager;

        uint32 m_nextID;

        std::unordered_map<uint32, Timer> m_timers;

        // timers in queue for deletion
        std::vector<uint32> m_deletionQueue;

        // groups and their pause state: true is paused, false is not
        std::unordered_map<TimerGroupID, bool> m_groups;

        void onAppUpdate(EVENT_HANDLER(Application));

        TimerID create(const TimeSpan &duration, TimerGroupID group);

        Timer *get(uint32 id);

        void cancel(uint32 id, bool invoke_removed = true);
    } Meta(Enable);
}