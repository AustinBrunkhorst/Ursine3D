/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

namespace ursine
{
    // invalid default constructor
    TimerID::TimerID(void)
        : m_id( -1 )
        , m_manager( nullptr ) { }

    TimerID::TimerID(TimerManager *manager, uint32 id)
        : m_id( id )
        , m_manager( manager ) { }

    bool TimerID::IsValid(void) const
    {
        return !!m_manager;
    }

    bool TimerID::IsActive(void) const
    {
        auto timer = m_manager->get( m_id );

        return timer && !timer->m_deleting;
    }

    bool TimerID::IsPaused(void) const
    {
        auto timer = m_manager->get( m_id );

        if (!timer)
            return true;

        return timer->m_paused || m_manager->m_groups[ timer->m_group ];
    }

    void TimerID::Pause(void)
    {
        auto timer = m_manager->get( m_id );

        if (timer)
            timer->m_paused = true;
    }

    void TimerID::Resume(void)
    {
        auto timer = m_manager->get( m_id );

        if (timer)
            timer->m_paused = false;
    }

    void TimerID::Cancel(void) const
    {
        m_manager->cancel( m_id );
    }

    TimerID &TimerID::Repeat(int count)
    {
        UAssert( count == Timer::REPEAT_FOREVER || count >= 0,
            "Timer repeation count must be non-negative." );

        auto timer = m_manager->get( m_id );

        if (timer)
            timer->m_repeat = count;

        return *this;
    }

    TimerID &TimerID::Repeated(TimerCallback callback)
    {
        auto timer = m_manager->get( m_id );

        if (timer)
            timer->m_repeated = callback;

        return *this;
    }

    TimerID &TimerID::Completed(TimerCallback callback)
    {
        auto timer = m_manager->get( m_id );

        if (timer)
            timer->m_completed = callback;

        return *this;
    }

    const TimeSpan &TimerID::GetDuration(void) const
    {
        auto timer = m_manager->get( m_id );

        return timer ? timer->m_duration : TimeSpan::Zero;
    }

    const TimeSpan &TimerID::GetElapsed(void) const
    {
        auto timer = m_manager->get( m_id );

        return timer ? timer->m_elapsed : TimeSpan::Zero;
    }

    TimerID &TimerID::Removed(TimerCallback callback)
    {
        auto timer = m_manager->get( m_id );

        if (timer)
            timer->_removed = callback;

        return *this;
    }
}
