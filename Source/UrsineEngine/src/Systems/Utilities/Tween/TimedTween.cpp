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

namespace Ursine
{
    TimedTween::TimedTween(const TimeSpan &duration)
        : _duration(duration)
        , _elapsed(TimeSpan::Zero)
    {
        
    }

    float TimedTween::GetPercentElapsed(void)
    {
        return static_cast<float>( _elapsed.Milliseconds( ) ) / _duration.Milliseconds( );
    }

    void TimedTween::elapse(float dt)
    {
        _elapsed += TimeSpan::FromSeconds( dt );
    }

    bool TimedTween::complete(void)
    {
        return _elapsed >= _duration;
    }
}
