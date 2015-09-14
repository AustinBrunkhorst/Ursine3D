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

namespace ursine
{
    LocalTimerManager::LocalTimerManager(TimerManager *manager, TimerGroupID default_group)
        : m_defaultGroup( default_group )
        , m_manager( manager ) { }

    LocalTimerManager::~LocalTimerManager(void)
    {
        ClearAll( );
    }

    TimerID LocalTimerManager::Create(const TimeSpan &duration)
    {
        return Create( duration, m_defaultGroup );
    }

    TimerID LocalTimerManager::Create(const TimeSpan &duration, TimerGroupID group)
    {
        auto timer = Timer::Create( duration, group );

        m_created[ timer.m_id ] = timer;

        return timer.Removed( [=] {
            m_created.erase( timer.m_id );
        } );
    }

    void LocalTimerManager::ClearAll(void)
    {
        for (auto &timer : m_created)
            m_manager->cancel( timer.second.m_id, false );

        if (m_created.size( ))
            m_created.clear( );
    }

    void LocalTimerManager::PauseAll(void)
    {
        for (auto &timer : m_created)
            timer.second.Pause( );
    }

    void LocalTimerManager::ResumeAll(void)
    {
        for (auto &timer : m_created)
            timer.second.Resume( );
    }

    void LocalTimerManager::SetDefaultGroup(TimerGroupID group)
    {
        m_defaultGroup = group;
    }
}
