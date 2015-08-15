/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ChainableEventOperator.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "EventConfig.h"
#include "EventArgs.h"

namespace Ursine
{
    template<typename DispatcherType, typename ListenerType>
    class ChainableEventOperator
    {
        // Eventable dispatching events
        DispatcherType *_dispatcher;

        // EventListener listening for events
        ListenerType *_listener;

    public:
        ChainableEventOperator(DispatcherType *dispatcher, ListenerType *listener);

        // Class member
        ChainableEventOperator &On(
            const typename DispatcherType::KeyType &type,
            ClassDelegate<ListenerType, EventArgs> delegate);

        // Static function
        ChainableEventOperator &On(
            const typename DispatcherType::KeyType &type,
            StaticDelegate<EventArgs> delegate);

        // Class member
        ChainableEventOperator &Off(
            const typename DispatcherType::KeyType &type,
            ClassDelegate<ListenerType, EventArgs> delegate);

        // Static function
        ChainableEventOperator &Off(
            const typename DispatcherType::KeyType &type,
            StaticDelegate<EventArgs> delegate);
    };
}

#include "ChainableEventOperator.hpp"
