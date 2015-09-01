/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ActionConfig.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <functional>

namespace ursine
{
    typedef std::function<void(void)> TweenCallback;

    typedef std::function<void(float)> TweenPercentageCallback;

    typedef uint32 TweenGroupID;

    const TweenGroupID kTweenGroupGameplay = 7;
}