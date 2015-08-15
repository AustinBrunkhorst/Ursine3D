/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TimerID.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "TimerConfig.h"

#include "UrsineTypes.h"

#include "TimeSpan.h"

namespace Ursine
{
    class TimerID
    {
        friend class TimerManager;
        friend class LocalTimerManager;

        // id of the timer in the manager
        uint32 _id;

        TimerID(uint32 id);
       
        // Sets the removed callback (used by LocalTimerManager)
        TimerID &Removed(TimerCallback callback);
        
    public:
        TimerID(void);

        // Determines if the timer representing this ID is active (in the TimerManager)
        bool IsActive(void);

        // Determines if the timer is paused
        bool IsPaused(void) const;

        // Sets a timer as paused
        void Pause(void);

        // Sets a timer as not paused
        void Resume(void);

        // Cancels the timer.
        void Cancel(void) const;

        // Sets the timer to repeat "count" times. use Timer::REPEAT_FOREVER to 
        // repeat until cancelled.
        TimerID &Repeat(int count);

        // Sets the repeated callback
        TimerID &Repeated(TimerCallback callback);

        // Sets the completion callback
        TimerID &Completed(TimerCallback callback);

        // Gets the duration of this timer
        const TimeSpan &GetDuration(void) const;

        // Gets how much time has elapsed for this timer in milliseconds
        const TimeSpan &GetElapsed(void) const;
    };
}