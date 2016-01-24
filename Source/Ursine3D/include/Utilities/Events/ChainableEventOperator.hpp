/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ChainableEventOperator.hpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

namespace ursine
{
    template<typename DispatcherType, typename ListenerType>
    ChainableEventOperator<DispatcherType, ListenerType>::ChainableEventOperator(DispatcherType *dispatcher, ListenerType *listener)
        : m_dispatcher( dispatcher )
        , m_listener( listener )
    {
        static_assert( std::is_constructible<typename DispatcherType::EventDispatcher::KeyType>::value,
            "dispatcher is not an Eventable." );
    }

    template<typename DispatcherType, typename ListenerType>
    ChainableEventOperator<DispatcherType, ListenerType> &ChainableEventOperator<DispatcherType, ListenerType>::On(
        const typename DispatcherType::EventDispatcher::KeyType &type,
        typename DispatcherType::EventDispatcher::HandlerType::template ClassDelegate<ListenerType, EventArgs> delegate,
        EventHandlerPriority priority
    )
    {
        m_dispatcher->template Connect<ListenerType, EventArgs>( type, m_listener, delegate, priority );

        return *this;
    }

    template<typename DispatcherType, typename ListenerType>
    ChainableEventOperator<DispatcherType, ListenerType> &ChainableEventOperator<DispatcherType, ListenerType>::On(
        const typename DispatcherType::EventDispatcher::KeyType &type,
        typename DispatcherType::EventDispatcher::HandlerType::template StaticDelegate<EventArgs> delegate,
        EventHandlerPriority priority
    )
    {
        m_dispatcher->template Connect<EventArgs>( type, delegate, priority );

        return *this;
    }

    template<typename DispatcherType, typename ListenerType>
    ChainableEventOperator<DispatcherType, ListenerType> &ChainableEventOperator<DispatcherType, ListenerType>::Off(
        const typename DispatcherType::EventDispatcher::KeyType &type,
        typename DispatcherType::EventDispatcher::HandlerType::template ClassDelegate<ListenerType, EventArgs> delegate
    )
    {
        m_dispatcher->template Disconnect<ListenerType, EventArgs>( type, m_listener, delegate );

        return *this;
    }

    template<typename DispatcherType, typename ListenerType>
    ChainableEventOperator<DispatcherType, ListenerType> &ChainableEventOperator<DispatcherType, ListenerType>::Off(
        const typename DispatcherType::EventDispatcher::KeyType &type,
        typename DispatcherType::EventDispatcher::HandlerType::template StaticDelegate<EventArgs> delegate
    )
    {
        m_dispatcher->template Disconnect<EventArgs>( type, delegate );

        return *this;
    }
}
