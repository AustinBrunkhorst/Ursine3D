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
    EventDispatcher<Key>::EventDispatcher(void)
        : m_defaultSender( nullptr )
    {
    }

    template<typename Key>
    EventDispatcher<Key>::EventDispatcher(void *default_sender)
        : m_defaultSender( default_sender )
    {
    }

    template<typename Key>
    template<typename Args>
    void EventDispatcher<Key>::Connect(const Key &event, StaticDelegate<Args> delegate)
    {
        // does this event already exist? 
        // If not, insert and construct the vector
        if (m_events.find( event ) == m_events.end( ))
            m_events.insert( std::make_pair( event, std::vector<Handler>( ) ) );

        m_events[ event ].push_back( Handler( delegate ) );
    }

    template<typename Key>
    template<typename Class, typename Args>
    void EventDispatcher<Key>::Connect(const Key &event, Class *context, ClassDelegate<Class, Args> delegate)
    {
        // does this event already exist? 
        // If not, insert and construct the vector
        if (m_events.find( event ) == m_events.end( ))
            m_events.insert( std::make_pair( event, std::vector<Handler>( ) ) );

        m_events[ event ].push_back( Handler( context, delegate ) );
    }

    template<typename Key>
    template<typename Args>
    void EventDispatcher<Key>::Disconnect(const Key &event, StaticDelegate<Args> delegate)
    {
        auto handlers = m_events.find( event );

        // this event doesn't have handlers
        if (handlers == m_events.end( ))
            return;

        auto &vector = handlers->second;

        auto handler = find( vector.begin( ), vector.end( ), Handler( delegate ) );

        if (handler != vector.end( ))
            vector.erase( handler );
    }

    template<typename Key>
    template<typename Class, typename Args>
    void EventDispatcher<Key>::Disconnect(const Key &event, Class *context, ClassDelegate<Class, Args> delegate)
    {
        auto handlers = m_events.find( event );

        // this event doesn't have handlers
        if (handlers == m_events.end( ))
            return;

        auto &vector = handlers->second;

        auto handler = find( vector.begin( ), vector.end( ), Handler( context, delegate ) );

        if (handler != vector.end( ))
            vector.erase( handler );
    }

    template<typename Key>
    void EventDispatcher<Key>::Dispatch(const Key &event, const EventArgs *args)
    {
        Dispatch( event, m_defaultSender, args );
    }

    template<typename Key>
    void EventDispatcher<Key>::Dispatch(const Key &event, void *sender, const EventArgs *args)
    {
        // this event doesn't have a handler
        if (m_events.find( event ) == m_events.end( ))
            return;

        auto handlers = m_events[ event ];

        for (auto it = handlers.begin( ); it != handlers.end( );)
        {
            (*it)( sender, args );

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
    ChainableEventOperator<EventDispatcher<Key>, ListenerType> EventDispatcher<Key>::Listener(ListenerType *listener)
    {
        return ChainableEventOperator<EventDispatcher<Key>, ListenerType>( this, listener );
    }
}
