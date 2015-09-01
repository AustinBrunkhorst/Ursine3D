/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Entity.hpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


#include "World.h"
#include "EntityManager.h"
#include "Utils.h"

namespace ursine
{
    namespace ecs
    {
        class EntityManager;

        template<class ComponentType, typename... Args>
        ComponentType *Entity::AddComponent(Args&&... args)
        {
            auto component = new ComponentType(std::forward<Args>(args)...);

            _world->Manager<EntityManager>()->AddComponent(this, component);

            return component;
        }

        template<class ComponentType>
        void Entity::RemoveComponent(void)
        {
            _world->Manager<EntityManager>()->RemoveComponent<ComponentType>(this);
        }

        template<class ComponentType>
        ComponentType *Entity::GetComponent(void) const
        {
            return _world->Manager<EntityManager>()->GetComponent<ComponentType>(this);
        }

        template<class ComponentType, typename... Args>
        void Entity::ApplyChildren(void (ComponentType::*func)(Args...), Args&&... args)
        {
            auto transform = GetTransform();

            if (transform)
                transform->ApplyChildren(func, std::forward<Args>(args)...);
        }

        template<class ComponentType>
        bool Entity::HasComponent(void) const
        {
            static const auto mask = GetComponentMask(ComponentType);

            return HasComponent(mask);
        }

        template<typename Args>
        void Entity::Connect(EventID event, StaticDelegate<Args> delegate)
        {
            _world->Manager<EntityManager>()->GetEntityEvents(this).Connect(event, delegate);
        }

        template<typename Class, typename Args>
        void Entity::Connect(EventID event, Class *context, ClassDelegate<Class, Args> delegate)
        {
            _world->Manager<EntityManager>()->GetEntityEvents(this).Connect(event, context, delegate);
        }

        template<typename Args>
        void Entity::Disconnect(EventID event, StaticDelegate<Args> delegate)
        {
            _world->Manager<EntityManager>()->GetEntityEvents(this).Disconnect(event, delegate);
        }

        template<typename Class, typename Args>
        void Entity::Disconnect(EventID event, Class *context, ClassDelegate<Class, Args> delegate)
        {
            _world->Manager<EntityManager>()->GetEntityEvents(this).Disconnect(event, context, delegate);
        }

        template<typename ListenerType>
        ChainableEventOperator<Entity, ListenerType> Entity::Listener(ListenerType *listener)
        {
            return ChainableEventOperator<Entity, ListenerType>(this, listener);
        }
    }
}