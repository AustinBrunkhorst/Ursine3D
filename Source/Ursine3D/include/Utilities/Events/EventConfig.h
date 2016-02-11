/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EventConfig.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "FastDelegate.h"
#include <functional>

namespace ursine
{
    struct EventArgs;

    struct FastDelegateHandler
    {
        using Handler = fastdelegate::FastDelegate<void(void*, const EventArgs *)>;

        template<typename Args = EventArgs>
        using StaticDelegate = void(*)(void *, const Args *);

        template<typename Class, typename Args = EventArgs>
        using ClassDelegate = void(Class::*)(void *, const Args *);
    };

    struct LambdaHandler
    {
        using Handler = std::function<void(void*, const EventArgs *)>;

        template<typename Args = EventArgs>
        using StaticDelegate = std::function<void(void *, const Args *)>;

        template<typename Class, typename Args = EventArgs>
        using ClassDelegate = void(Class::*)(void *, const Args *);
    };

    typedef int EventHandlerPriority;

    const EventHandlerPriority kDefaultEventHandlerPriority = 0;
}
