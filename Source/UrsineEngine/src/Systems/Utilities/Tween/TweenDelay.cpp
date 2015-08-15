/* ---------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ActionDelay.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "UrsinePrecompiled.h"

#include "TweenDelay.h"

namespace Ursine
{
    TweenDelay::TweenDelay(const TimeSpan &duration)
        : TimedTween(duration)
    {
        
    }

    bool TweenDelay::Update(void)
    {
        elapse(gApplication->GetDelta());

        return complete();
    }
}