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

namespace ursine
{
    TweenProperty::TweenProperty(const TimeSpan &duration, TweenPercentageCallback updater)
        : TimedTween( duration )
        , m_propertyUpdater( updater )
    {

    }

    bool TweenProperty::Update(DeltaTime dt)
    {
        elapse( dt );

        m_propertyUpdater( GetPercentElapsed( ) );

        if (complete( ))
        {
            m_propertyUpdater( 1.0f );

            return true;
        }

        return false;
    }
}