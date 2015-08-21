/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TimerManager.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "TimerManager.h"
#include "TimerID.h"

namespace Ursine
{
    TimerManager *gTimerManager = nullptr;

    TimerManager::TimerManager(void)
        : _next_id(0)
    {
        gTimerManager = this;
    }

    TimerManager::~TimerManager(void)
    {
        gTimerManager = nullptr;
    }

    void TimerManager::Update(void)
    {
        // dt is in terms of seconds
        auto dt = gApplication->GetDelta() * TimeSpan::MillisPerSecond;

        for (auto &pair : _timers)
        {
            auto &timer = pair.second;

            // deleting or group paused
            if (timer._deleting || timer._paused || _groups[timer._group])
                continue;

            timer._elapsed += dt;

            if (timer._elapsed >= timer._duration)
            {
                if (timer._repeated)
                    timer._repeated();

                timer._elapsed = 0.0f;

                // we're repeating forever
                if (timer._repeat == Timer::REPEAT_FOREVER)
                    continue;

                // are we done repeating?
                if (--timer._repeat <= 0)
                {
                    if (timer._completed)
                        timer._completed();

                    cancel(pair.first);
                }
            }
        }

        for (auto &deleted : _deletion_queue)
            _timers.erase(deleted);

        _deletion_queue.clear();
    }

    void TimerManager::Pause(TimerGroupID group)
    {
        _groups[group] = true;
    }

    void TimerManager::Resume(TimerGroupID group)
    {
        _groups[group] = false;
    }

    void TimerManager::Clear(TimerGroupID group)
    {
        for (auto it = _timers.begin(); it != _timers.end(); )
        {
            if (it->second._group == group)
                _timers.erase(it++);
            else
                ++it;
        }
    }

    TimerID TimerManager::create(const TimeSpan &duration, TimerGroupID group)
    {
        auto id = _next_id++;

        _timers.emplace(std::make_pair(id, Timer(duration, group)));

        return TimerID(id);
    }

    Timer *TimerManager::get(uint32 id)
    {
        auto search = _timers.find(id);

        return search == _timers.end() ? nullptr : &search->second;
    }

    void TimerManager::cancel(uint32 id, bool invoke_removed)
    {
        auto timer = get(id);

        if (timer && !timer->_deleting)
        {
            if (invoke_removed && timer->_removed)
                timer->_removed();

            timer->_deleting = true;

            _deletion_queue.push_back(id);
        }
    }
}