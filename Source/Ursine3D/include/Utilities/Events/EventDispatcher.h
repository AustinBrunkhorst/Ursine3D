/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EventDispatcher.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "EventConfig.h"

#include "ChainableEventOperator.h"
#include "Utils.h"

#include <string>
#include <unordered_map>

// Lists the parameters for an Eventable type
#define EVENT_HANDLER(type) void *_sender, const ursine::EventArgs *_args

// Casts an event sender to the appropriate type
#define EVENT_SENDER(type, name) type *name = static_cast<type *>(_sender)

// Casts an event's arguments to the appropriate type
#define EVENT_ARGS(type, name) const type *name = static_cast<const type *>(_args)

// Automatically casts the event parameters to the appropriate type (Sender, Arguments)
#define EVENT_ATTRS(send_type, arg_type) EVENT_SENDER(send_type, sender); EVENT_ARGS(arg_type, args)

namespace ursine
{
    template<
        typename Key = std::string,
        typename Handler = FastDelegateHandler
    >
    class EventDispatcher
    {
    public:
        typedef Key KeyType;
        typedef Handler HandlerType;

        template<typename Listener>
        using ChainType = ChainableEventOperator<EventDispatcher<Key>, Listener>;

        EventDispatcher(void *defaultSender = nullptr);

        // Binds a connection to a static function
        template<typename Args = EventArgs>
        void Connect(
            const Key &event,
            typename Handler::template StaticDelegate<Args> delegate, 
            EventHandlerPriority priority = kDefaultEventHandlerPriority
        );

        // Binds a connection to a class member function
        template<typename Class, typename Args = EventArgs>
        void Connect(
            const Key &event, 
            Class *context, 
            typename Handler::template ClassDelegate<Class, Args> delegate,
            EventHandlerPriority priority = kDefaultEventHandlerPriority
        );

        // Removes a bound static function from this dispatcher
        template<typename Args = EventArgs>
        void Disconnect(
            const Key &event, 
            typename Handler::template StaticDelegate<Args> delegate
        );

        // Removes a bound class member function from this dispatcher
        template<typename Class, typename Args = EventArgs>
        void Disconnect(
            const Key &event,
            Class *context, 
            typename Handler::template ClassDelegate<Class, Args> delegate
        );

        // Dispatches the given event with the given args. "this" is used for the sender.
        void Dispatch(
            const Key &event,
            const EventArgs *args
        ) const;

        // Dispatches the given event with the given args.
        void Dispatch(
            const Key &event, 
            void *sender, 
            const EventArgs *args
        ) const;

        // Clears all handlers from this dispatcher (effectively resets)
        void ClearHandlers(void);

        // Creates a Chainable Event Operator based on the listener. By default,
        // the listener is nullptr, so it is assumed that all of the chaining
        // operations will be from static functions otherwise you'll be 
        // passing a nullptr as the class context (bad).
        template<typename Listener>
        ChainType<Listener> Listener(Listener *listener = nullptr);

    private:
        void *m_defaultSender;

        struct HandlerData
        {
            EventHandlerPriority priority;
            typename Handler::Handler handler;
        };

        typedef std::vector<HandlerData> HandlerListType;

        // hash functor that handles integral types
        struct IntegralHasher
        {
            template <typename T>
            size_t operator()(T t) const
            {
                return static_cast<size_t>( t );
            }
        };

        // functor type that uses "IntegralHasher" if "T" is an enum, otherwise the default
        template <typename T>
        using HashHandlerType = typename std::conditional< 
            std::is_enum<T>::value, IntegralHasher, 
            std::hash<T> 
        >::type;

        std::unordered_map<
            Key, 
            HandlerListType,
            HashHandlerType<Key>
        > m_events;

        inline static bool compareHandlers(
            const HandlerData &a, 
            const HandlerData &b
        );
    };
}

#include "EventDispatcher.hpp"