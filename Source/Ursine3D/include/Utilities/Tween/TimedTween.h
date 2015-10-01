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
    public:
        float GetPercentElapsed(void);

    protected:
        TimeSpan m_duration;
        TimeSpan m_elapsed;

        void elapse(DeltaTime dt);

        bool complete(void);

        TimedTween(const TimeSpan &duration);
    };
}