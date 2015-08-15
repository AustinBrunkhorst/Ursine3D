/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** LocalTimerManager.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "LocalTimerManager.h"

#include "TimerManager.h"

namespace Ursine
{
    LocalTimerManager::LocalTimerManager(TimerGroupID default_group)
        : _default_group(default_group)
    {
        
    }

    LocalTimerManager::~LocalTimerManager(void)
    {
        ClearAll();
    }

    TimerID LocalTimerManager::Create(const TimeSpan &duration)
    {
        return Create(duration, _default_group);
    }

    TimerID LocalTimerManager::Create(const TimeSpan &duration, TimerGroupID group)
    {
        auto timer = Timer::Create(duration, group);

        _created[timer._id] = timer;

        return timer.Removed([=] {
            _created.erase(timer._id);
        });
    }

    void LocalTimerManager::ClearAll(void)
    {
        for (auto &timer : _created)
            gTimerManager->cancel(timer.second._id, false);
        
        if (_created.size())
            _created.clear();
    }

    void LocalTimerManager::PauseAll(void)
    {
        for (auto &timer : _created)
            timer.second.Pause();
    }

    void LocalTimerManager::ResumeAll(void)
    {
        for (auto &timer : _created)
            timer.second.Resume();
    }

    void LocalTimerManager::SetDefaultGroup(TimerGroupID group)
    {
        _default_group = group;
    }
}