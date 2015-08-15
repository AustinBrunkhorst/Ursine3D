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

namespace Ursine
{
    template<typename Key>
    EventDispatcher<Key>::EventDispatcher(void)
        : _default_sender(nullptr) {}

    template<typename Key>
    EventDispatcher<Key>::EventDispatcher(void *default_sender)
        : _default_sender(default_sender) {}

    template<typename Key>
    template<typename Args>
    void EventDispatcher<Key>::Connect(const Key &event, StaticDelegate<Args> delegate)
    {
        // does this event already exist? 
        // If not, insert and construct the vector
        if (_events.find(event) == _events.end())
            _events.insert(std::make_pair(event, std::vector<Handler>()));

        _events[event].push_back(Handler(delegate));
    }

    template<typename Key>
    template<typename Class, typename Args>
    void EventDispatcher<Key>::Connect(const Key &event, Class *context, ClassDelegate<Class, Args> delegate)
    {
        // does this event already exist? 
        // If not, insert and construct the vector
        if (_events.find(event) == _events.end())
            _events.insert(std::make_pair(event, std::vector<Handler>()));

        _events[event].push_back(Handler(context, delegate));
    }

    template<typename Key>
    template<typename Args>
    void EventDispatcher<Key>::Disconnect(const Key &event, StaticDelegate<Args> delegate)
    {
        auto handlers = _events.find(event);

        // this event doesn't have handlers
        if (handlers == _events.end())
            return;

        auto &vector = handlers->second;

        auto handler = find(vector.begin(), vector.end(), Handler(delegate));

        if (handler != vector.end())
            vector.erase(handler);
    }

    template<typename Key>
    template<typename Class, typename Args>
    void EventDispatcher<Key>::Disconnect(const Key &event, Class *context, ClassDelegate<Class, Args> delegate)
    {
        auto handlers = _events.find(event);

        // this event doesn't have handlers
        if (handlers == _events.end())
            return;

        auto &vector = handlers->second;

        auto handler = find(vector.begin(), vector.end(), Handler(context, delegate));

        if (handler != vector.end())
            vector.erase(handler);
    }

    template<typename Key>
    void EventDispatcher<Key>::Dispatch(const Key &event, const EventArgs *args)
    {
        Dispatch(event, _default_sender, args);
    }

    template<typename Key>
    void EventDispatcher<Key>::Dispatch(const Key &event, void *sender, const EventArgs *args)
    {
        // this event doesn't have a handler
        if (_events.find(event) == _events.end())
            return;

        auto handlers = _events[event];

        for (auto it = handlers.begin(); it != handlers.end(); )
        {
            (*it)(sender, args);

            ++it;
        }
    }

    template<typename Key>
    void EventDispatcher<Key>::ClearHandlers(void)
    {
        _events.clear();
    }

    template<typename Key>
    template<typename ListenerType>
    ChainableEventOperator<EventDispatcher<Key>, ListenerType> EventDispatcher<Key>::Listener(ListenerType *listener)
    {
        return ChainableEventOperator<EventDispatcher<Key>, ListenerType>(this, listener);
    }
}