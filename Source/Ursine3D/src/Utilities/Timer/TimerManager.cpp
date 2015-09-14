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

namespace ursine
{
    CORE_SYSTEM_DEFINITION( TimerManager );

    TimerManager::TimerManager(void)
        : m_nextID( 0 )
    {
        Application::Instance->Connect( APP_UPDATE, this, &TimerManager::onAppUpdate );
    }

    TimerManager::~TimerManager(void)
    {
        Application::Instance->Disconnect( APP_UPDATE, this, &TimerManager::onAppUpdate );
    }

    void TimerManager::Pause(TimerGroupID group)
    {
        m_groups[ group ] = true;
    }

    void TimerManager::Resume(TimerGroupID group)
    {
        m_groups[ group ] = false;
    }

    void TimerManager::Clear(TimerGroupID group)
    {
        for (auto it = m_timers.begin( ); it != m_timers.end( );)
        {
            if (it->second.m_group == group)
                m_timers.erase( it++ );
            else
                ++it;
        }
    }

    void TimerManager::onAppUpdate(EVENT_HANDLER(Application))
    {
        EVENT_ATTRS(Application, EventArgs);

        auto dt = sender->GetDeltaTime( );

        for (auto &pair : m_timers)
        {
            auto &timer = pair.second;

            // deleting or group paused
            if (timer.m_deleting || timer.m_paused || m_groups[ timer.m_group ])
                continue;

            timer.m_elapsed += dt;

            if (timer.m_elapsed >= timer.m_duration)
            {
                if (timer.m_repeated)
                    timer.m_repeated( );

                timer.m_elapsed = 0.0f;

                // we're repeating forever
                if (timer.m_repeat == Timer::REPEAT_FOREVER)
                    continue;

                // are we done repeating?
                if (--timer.m_repeat <= 0)
                {
                    if (timer.m_completed)
                        timer.m_completed( );

                    cancel( pair.first );
                }
            }
        }

        for (auto &deleted : m_deletionQueue)
            m_timers.erase( deleted );

        m_deletionQueue.clear( );
    }

    TimerID TimerManager::create(const TimeSpan &duration, TimerGroupID group)
    {
        auto id = m_nextID++;

        m_timers.emplace( std::make_pair( id, Timer( duration, group ) ) );

        return TimerID( this, id );
    }

    Timer *TimerManager::get(uint32 id)
    {
        auto search = m_timers.find( id );

        return search == m_timers.end( ) ? nullptr : &search->second;
    }

    void TimerManager::cancel(uint32 id, bool invoke_removed)
    {
        auto timer = get( id );

        if (timer && !timer->m_deleting)
        {
            if (invoke_removed && timer->_removed)
                timer->_removed( );

            timer->m_deleting = true;

            m_deletionQueue.push_back( id );
        }
    }
}
