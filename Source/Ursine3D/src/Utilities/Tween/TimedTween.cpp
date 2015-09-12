/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TimedAction.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "TimedTween.h"

namespace ursine
{
    TimedTween::TimedTween(const TimeSpan &duration)
        : m_duration( duration )
        , m_elapsed( TimeSpan::Zero ) { }

    float TimedTween::GetPercentElapsed(void)
    {
        return static_cast<float>( m_elapsed.Milliseconds( ) ) / m_duration.Milliseconds( );
    }

    void TimedTween::elapse(DeltaTime dt)
    {
        m_elapsed += TimeSpan::FromSeconds( dt );
    }

    bool TimedTween::complete(void)
    {
        return m_elapsed >= m_duration;
    }
}
