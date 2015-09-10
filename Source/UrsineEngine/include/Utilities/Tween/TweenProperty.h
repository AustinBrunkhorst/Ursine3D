/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ActionProperty.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "TimedTween.h"

namespace ursine
{
    class TweenProperty :
        public TimedTween,
        public TweenItem
    {
        TweenPercentageCallback _property_update;

    public:
        TweenProperty(const TimeSpan &duration, TweenPercentageCallback updater);

        bool Update(void) override;
    };
}