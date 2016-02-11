/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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
    template<typename Key, typename Handler>
    EventDispatcher<Key, Handler>::EventDispatcher(void *defaultSender /*= nullptr*/)
        : m_defaultSender( defaultSender ) { }

    template<typename Key, typename Handler>
    template<typename Args>
    void EventDispatcher<Key, Handler>::Connect(
        const Key &event,
        typename Handler::template StaticDelegate<Args> delegate,
        EventHandlerPriority priority
    )
    {
        auto &handlers = m_events[ event ];

        HandlerData data { priority, Handler::Handler( delegate ) };

        utils::InsertionSort( handlers, data, compareHandlers );
    }

    template<typename Key, typename Handler>
    template<typename Class, typename Args>
    void EventDispatcher<Key, Handler>::Connect(
        const Key &event,
        Class *context,
        typename Handler::template ClassDelegate<Class, Args> delegate,
        EventHandlerPriority priority
    )
    {
        auto &handlers = m_events[ event ];

        HandlerData data { priority, Handler::Handler( context, delegate ) };

        utils::InsertionSort( handlers, data, compareHandlers );
    }

    template<typename Key, typename Handler>
    template<typename Args>
    void EventDispatcher<Key, Handler>::Disconnect(
        const Key &event,
        typename Handler::template StaticDelegate<Args> delegate
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

    template<typename Key, typename Handler>
    template<typename Class, typename Args>
    void EventDispatcher<Key, Handler>::Disconnect(
        const Key &event,
        Class *context,
        typename Handler::template ClassDelegate<Class, Args> delegate
    )
    {
        auto handlers = m_events.find( event );

        // this event doesn't have handlers
        if (handlers == m_events.end( ))
            return;

        auto &vector = handlers->second;

        auto handlerType = Handler::Handler( context, delegate );

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

    template<typename Key, typename Handler>
    void EventDispatcher<Key, Handler>::Dispatch(
        const Key &event,
        const EventArgs *args
    )
    {
        Dispatch( event, m_defaultSender, args );
    }

    template<typename Key, typename Handler>
    void EventDispatcher<Key, Handler>::Dispatch(
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

    template<typename Key, typename Handler>
    void EventDispatcher<Key, Handler>::ClearHandlers(void)
    {
        m_events.clear( );
    }

    template<typename Key, typename Handler>
    template<typename ListenerType>
    typename EventDispatcher<Key, Handler>::ChainType<ListenerType>
    EventDispatcher<Key, Handler>::Listener(ListenerType *listener /*= nullptr*/)
    {
        return ChainType<ListenerType>( this, listener );
    }

    template<typename Key, typename Handler>
    bool EventDispatcher<Key, Handler>::compareHandlers(
        const HandlerData &a, 
        const HandlerData &b
    )
    {
        // note: descending order
        return b.priority < a.priority;
    }
}
