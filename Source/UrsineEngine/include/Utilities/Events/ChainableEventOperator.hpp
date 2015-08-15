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

namespace Ursine
{
    template<typename DispatcherType, typename ListenerType>
    ChainableEventOperator<DispatcherType, ListenerType>::ChainableEventOperator(DispatcherType *dispatcher, ListenerType *listener)
        : _dispatcher(dispatcher)
        , _listener(listener)
    {
        static_assert(std::is_constructible<typename DispatcherType::KeyType>::value,
            "dispatcher is not an Eventable.");
    }

    template<typename DispatcherType, typename ListenerType>
    ChainableEventOperator<DispatcherType, ListenerType> &ChainableEventOperator<DispatcherType, ListenerType>::On(
        const typename DispatcherType::KeyType &type,
        ClassDelegate<ListenerType, EventArgs> delegate)
    {
        _dispatcher->template Connect<ListenerType, EventArgs>(type, _listener, delegate);

        return *this;
    }

    template<typename DispatcherType, typename ListenerType>
    ChainableEventOperator<DispatcherType, ListenerType> &ChainableEventOperator<DispatcherType, ListenerType>::On(
        const typename DispatcherType::KeyType &type,
        StaticDelegate<EventArgs> delegate)
    {
        _dispatcher->template Connect<EventArgs>(type, delegate);

        return *this;
    }

    template<typename DispatcherType, typename ListenerType>
    ChainableEventOperator<DispatcherType, ListenerType> &ChainableEventOperator<DispatcherType, ListenerType>::Off(
        const typename DispatcherType::KeyType &type,
        ClassDelegate<ListenerType, EventArgs> delegate)
    {
        _dispatcher->template Disconnect<ListenerType, EventArgs>(type, _listener, delegate);

        return *this;
    }

    template<typename DispatcherType, typename ListenerType>
    ChainableEventOperator<DispatcherType, ListenerType> &ChainableEventOperator<DispatcherType, ListenerType>::Off(
        const typename DispatcherType::KeyType &type,
        StaticDelegate<EventArgs> delegate)
    {
        _dispatcher->template Disconnect<EventArgs>(type, delegate);

        return *this;
    }
}