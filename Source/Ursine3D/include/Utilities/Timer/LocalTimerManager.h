/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** LocalTimerManager.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Timer.h"

namespace ursine
{
    class LocalTimerManager
    {
    public:
        LocalTimerManager(void) = default;

        LocalTimerManager(TimerManager *manager, TimerGroupID defaultGroup = Timer::GROUP_DEFAULT);
        ~LocalTimerManager(void);

        // Creates a timer with the default group defined during construction
        TimerID Create(const TimeSpan &duration);

        // Creates a timer with the specified group
        TimerID Create(const TimeSpan &duration, TimerGroupID group);

        // Clears all timers in this manager
        void ClearAll(void);

        // Pauses all timers in this manager
        void PauseAll(void);

        // Resumes all timers in this manager
        void ResumeAll(void);

        // Sets the default group ID for this local manager
        void SetDefaultGroup(TimerGroupID group);

    private:
        TimerGroupID m_defaultGroup;

        TimerManager *m_manager;

        std::unordered_map<uint32, TimerID> m_created;
    };
}