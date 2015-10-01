/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Timer.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Timer.h"
#include "TimerManager.h"

namespace ursine
{
    Timer::Timer(const TimeSpan &duration, TimerGroupID group)
        : m_group( group )
        , m_repeat( 0 )
        , m_deleting( false )
        , m_paused( false )
        , m_duration( duration )
        , m_elapsed( 0.0f )
        , m_repeated( nullptr )
        , m_completed( nullptr )
        , _removed( nullptr ) { }

    Timer::~Timer(void) { }

    TimerID Timer::Create(const TimeSpan &duration, TimerGroupID group)
    {
        static auto *manager = GetCoreSystem( TimerManager );

        UAssert (duration >= TimeSpan::Zero,
            "Timer duration must be non-negative." );

        return manager->create( duration, group );
    }
}
