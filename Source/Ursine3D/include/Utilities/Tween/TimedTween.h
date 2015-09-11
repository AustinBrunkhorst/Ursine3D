/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TimedAction.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    class TimedTween
    {
    protected:
        TimeSpan _duration;
        TimeSpan _elapsed;

        void elapse(float dt);

        bool complete(void);

        TimedTween(const TimeSpan &duration);
    public:
        float GetPercentElapsed(void);
    };
}