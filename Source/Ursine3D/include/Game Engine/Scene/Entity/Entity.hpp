/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

namespace ursine
{
    namespace ecs
    {
        template<class ComponentType, typename... Args>
        ComponentType *Entity::AddComponent(Args &&... args)
        {
            auto component = new ComponentType( std::forward<Args>( args )... );

            m_world->m_entityManager->AddComponent( this, component );

            return component;
        }

        template<class ComponentType>
        void Entity::RemoveComponent(void)
        {
            m_world->m_entityManager->RemoveComponent<ComponentType>( this );
        }

        template<class ComponentType>
        ComponentType *Entity::GetComponent(void) const
        {
            static_assert( !std::is_same<ComponentType, Transform>::value, 
                "Use GetTransform( ) to get the Transform component." );

            return m_world->m_entityManager->GetComponent<ComponentType>( this );
        }

        template<class ComponentType>
        bool Entity::HasComponent(void) const
        {
            static const auto &mask = GetComponentMask( ComponentType );

            return HasComponent( mask );
        }

	    template <class ComponentType>
	    ComponentType* Entity::GetComponentInChildren(void) const
	    {
			return m_world->m_entityManager->GetComponentInChildren<ComponentType>( this );
	    }

	    template <class ComponentType>
	    ComponentType* Entity::GetComponentInParent(void) const
	    {
			return m_world->m_entityManager->GetComponentInParent<ComponentType>( this );
	    }

	    template <class ComponentType>
	    std::vector<ComponentType*> Entity::GetComponentsInChildren(void) const
	    {
			return m_world->m_entityManager->GetComponentsInChildren<ComponentType>( this );
	    }

	    template <class ComponentType>
	    std::vector<ComponentType*> Entity::GetComponentsInParents(void) const
	    {
			return m_world->m_entityManager->GetComponentsInParents<ComponentType>( this );
	    }

	    template<typename Args>
        void Entity::Connect(
            EventID event, 
            EventDispatcher::HandlerType::StaticDelegate<Args> delegate,
            EventHandlerPriority priority
        )
        {
            m_world->m_entityManager->GetEntityEvents( this )
                .Connect( event, delegate, priority );
        }

        template<typename Class, typename Args>
        void Entity::Connect(
            EventID event, 
            Class *context, 
            EventDispatcher::HandlerType::ClassDelegate<Class, Args> delegate,
            EventHandlerPriority priority
        )
        {
            m_world->m_entityManager->GetEntityEvents( this )
                .Connect( event, context, delegate, priority );
        }

        template<typename Args>
        void Entity::Disconnect(EventID event, EventDispatcher::HandlerType::StaticDelegate<Args> delegate)
        {
            m_world->m_entityManager->GetEntityEvents( this )
                .Disconnect( event, delegate );
        }

        template<typename Class, typename Args>
        void Entity::Disconnect(
            EventID event, 
            Class *context, 
            EventDispatcher::HandlerType::ClassDelegate<Class, Args> delegate
        )
        {
            m_world->m_entityManager->GetEntityEvents( this )
                .Disconnect( event, context, delegate );
        }

        template<typename ListenerType>
        ChainableEventOperator<Entity, ListenerType> Entity::Listener(ListenerType *listener)
        {
            return ChainableEventOperator<Entity, ListenerType>( this, listener );
        }
    }
}
