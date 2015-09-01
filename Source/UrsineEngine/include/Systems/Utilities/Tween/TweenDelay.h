/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ActionDelay.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "TweenItem.h"
#include "TimedTween.h"

namespace ursine
{
    class TweenDelay 
        : public TimedTween
        , public TweenItem
    {
    public:
        explicit TweenDelay(const TimeSpan &duration);

        bool Update(void) override;
    };
}