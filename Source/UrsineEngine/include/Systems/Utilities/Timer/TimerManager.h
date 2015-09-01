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

#include "System.h"

#include "Timer.h"

namespace ursine
{
    class TimerManager : public System
    {
        friend class Timer;
        friend class TimerID;

        friend class LocalTimerManager;

        uint32 _next_id;

        std::unordered_map<uint32, Timer> _timers;

        // timers in queue for deletion
        std::vector<uint32> _deletion_queue;

        // groups and their pause state: true is paused, false is not
        std::unordered_map<TimerGroupID, bool> _groups;

        TimerID create(const TimeSpan &duration, TimerGroupID group);

        Timer *get(uint32 id);

        void cancel(uint32 id, bool invoke_removed = true);

    public:
        TimerManager(void);
        ~TimerManager(void);

        void Update(void) override;

        // Suspends updating a specific group
        void Pause(TimerGroupID group);

        // Resumes updating a specific group
        void Resume(TimerGroupID group);

        // Removes all timers in a specific group
        void Clear(TimerGroupID group = Timer::GROUP_DEFAULT);
    };

    extern TimerManager *gTimerManager;
}