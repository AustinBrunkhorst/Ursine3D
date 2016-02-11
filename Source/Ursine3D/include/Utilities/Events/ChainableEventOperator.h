/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

namespace ursine
{
    template<typename DispatcherType, typename ListenerType>
    class ChainableEventOperator
    {
        // Eventable dispatching events
        DispatcherType *m_dispatcher;

        // EventListener listening for events
        ListenerType *m_listener;

    public:
        ChainableEventOperator(DispatcherType *dispatcher, ListenerType *listener);

        // Class member
        ChainableEventOperator &On(
            const typename DispatcherType::EventDispatcher::KeyType &type,
            typename DispatcherType::EventDispatcher::HandlerType::template ClassDelegate<ListenerType, EventArgs> delegate,
            EventHandlerPriority priority = kDefaultEventHandlerPriority
        );

        // Static function
        ChainableEventOperator &On(
            const typename DispatcherType::EventDispatcher::KeyType &type,
            typename DispatcherType::EventDispatcher::HandlerType::template StaticDelegate<EventArgs> delegate,
            EventHandlerPriority priority = kDefaultEventHandlerPriority
        );

        // Class member
        ChainableEventOperator &Off(
            const typename DispatcherType::EventDispatcher::KeyType &type,
            typename DispatcherType::EventDispatcher::HandlerType::template ClassDelegate<ListenerType, EventArgs> delegate
        );

        // Static function
        ChainableEventOperator &Off(
            const typename DispatcherType::EventDispatcher::KeyType &type,
            typename DispatcherType::EventDispatcher::HandlerType::template StaticDelegate<EventArgs> delegate
        );
    };
}

#include "ChainableEventOperator.hpp"
