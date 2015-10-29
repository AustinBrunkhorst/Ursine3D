/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EventDispatcher.hpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

namespace ursine
{
    template<typename Key>
    EventDispatcher<Key>::EventDispatcher(void *defaultSender /*= nullptr*/)
        : m_defaultSender( defaultSender ) { }

    template<typename Key>
    template<typename Args>
    void EventDispatcher<Key>::Connect(
        const Key &event,
        StaticDelegate<Args> delegate,
        EventHandlerPriority priority
    )
    {
        auto &handlers = m_events[ event ];

        HandlerData data { priority, Handler( delegate ) };

        utils::InsertionSort( handlers, data, compareHandlers );
    }

    template<typename Key>
    template<typename Class, typename Args>
    void EventDispatcher<Key>::Connect(
        const Key &event,
        Class *context,
        ClassDelegate<Class, Args> delegate,
        EventHandlerPriority priority
    )
    {
        auto &handlers = m_events[ event ];

        HandlerData data { priority, Handler( context, delegate ) };

        utils::InsertionSort( handlers, data, compareHandlers );
    }

    template<typename Key>
    template<typename Args>
    void EventDispatcher<Key>::Disconnect(
        const Key &event,
        StaticDelegate<Args> delegate
    )
    {
        auto handlers = m_events.find( event );

        // this event doesn't have handlers
        if (handlers == m_events.end( ))
            return;

        auto &vector = handlers->second;

        auto handlerType = Handler( delegate );

        auto isHandler = [&](const HandlerData &data)
        {
            return data.handler == handlerType;
        };

        auto handler = std::find_if(
            vector.begin( ),
            vector.end( ),
            isHandler
        );

        if (handler != vector.end( ))
            vector.erase( handler );
    }

    template<typename Key>
    template<typename Class, typename Args>
    void EventDispatcher<Key>::Disconnect(
        const Key &event,
        Class *context,
        ClassDelegate<Class, Args> delegate
    )
    {
        auto handlers = m_events.find( event );

        // this event doesn't have handlers
        if (handlers == m_events.end( ))
            return;

        auto &vector = handlers->second;

        auto handlerType = Handler( context, delegate );

        auto isHandler = [&](const HandlerData &data)
        {
            return data.handler == handlerType;
        };

        auto handler = std::find_if( 
            vector.begin( ), 
            vector.end( ), 
            isHandler 
        );

        if (handler != vector.end( ))
            vector.erase( handler );
    }

    template<typename Key>
    void EventDispatcher<Key>::Dispatch(
        const Key &event,
        const EventArgs *args
    )
    {
        Dispatch( event, m_defaultSender, args );
    }

    template<typename Key>
    void EventDispatcher<Key>::Dispatch(
        const Key &event,
        void *sender,
        const EventArgs *args
    )
    {
        // this event doesn't have a handler
        if (m_events.find( event ) == m_events.end( ))
            return;

        auto handlers = m_events[ event ];

        for (auto it = handlers.begin( ); it != handlers.end( );)
        {
            (*it).handler( sender, args );

            ++it;
        }
    }

    template<typename Key>
    void EventDispatcher<Key>::ClearHandlers(void)
    {
        m_events.clear( );
    }

    template<typename Key>
    template<typename ListenerType>
    typename EventDispatcher<Key>::ChainType<ListenerType>
    EventDispatcher<Key>::Listener(ListenerType *listener /*= nullptr*/)
    {
        return ChainType<ListenerType>( this, listener );
    }

    template<typename Key>
    bool EventDispatcher<Key>::compareHandlers(
        const HandlerData &a, 
        const HandlerData &b
    )
    {
        // note: descending order
        return b.priority < a.priority;
    }
}
