/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ActionProperty.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "UrsinePrecompiled.h"

#include "TweenProperty.h"

namespace Ursine
{
    TweenProperty::TweenProperty(const TimeSpan &duration, TweenPercentageCallback updater)
        : TimedTween(duration)
        , _property_update(updater)
    {

    }

    bool TweenProperty::Update(void)
    {
        elapse( gApplication->GetDelta( ) );

        _property_update( GetPercentElapsed( ) );

        if (complete())
        {
            _property_update( 1.0f );

            return true;
        }

        return false;
    }
}