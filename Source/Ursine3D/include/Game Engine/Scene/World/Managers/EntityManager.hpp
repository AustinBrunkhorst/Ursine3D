/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityManager.hpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Component.h"

namespace ursine
{
    namespace ecs
    {
        template<class ComponentType>
        void EntityManager::AddComponent(Entity *entity, ComponentType *component)
        {
            static_assert(std::is_base_of<Component, ComponentType>::value,
                "The specified type is not a component!");

#ifdef CONFIG_DEBUG

            UAssert( IsComponentRegistered( ComponentType ),
                "Component type not registered." );

#endif

            AddComponent( entity, static_cast<Component*>( component ) );
        }

        template<class ComponentType>
        void EntityManager::RemoveComponent(Entity *entity)
        {
            static_assert(!std::is_same<ComponentType, Transform>::value,
                "You can't remove a transform component." );

            RemoveComponent( entity, GetComponentID( ComponentType ) );
        }

        template<class ComponentType>
        ComponentType *EntityManager::GetComponent(const Entity *entity) const
        {
            return static_cast<ComponentType*>( 
                GetComponent( 
                    entity,                
                    GetComponentID( ComponentType ) 
                ) 
            );
        }

        template <class ComponentType>
        ComponentType* EntityManager::GetComponentInChildren(const Entity* entity) const
        {
            return static_cast<ComponentType*>(GetComponentInChildren(entity,
                GetComponentID(ComponentType)));
        }

        template <class ComponentType>
        ComponentType* EntityManager::GetComponentInParent(const Entity* entity) const
        {
            return static_cast<ComponentType*>(GetComponentInParent(entity,
                GetComponentID(ComponentType)));
        }

        template <class ComponentType>
        std::vector<ComponentType*> EntityManager::GetComponentsInChildren(const Entity* entity) const
        {
            auto components = GetComponentsInChildren( entity, GetComponentID( ComponentType ) );

            std::vector<ComponentType*> derivedVector;

            for (auto &component : components)
                derivedVector.push_back( static_cast<ComponentType*>( component ) );
            
            return derivedVector;
        }

        template <class ComponentType>
        std::vector<ComponentType*> EntityManager::GetComponentsInParents(const Entity* entity) const
        {
            auto components = GetComponentsInParents( entity, GetComponentID( ComponentType ) );

            std::vector<ComponentType*> derivedVector;

            for (auto &component : components)
                derivedVector.push_back( static_cast<ComponentType*>( component ) );
            
            return derivedVector;
        }
    }
}
