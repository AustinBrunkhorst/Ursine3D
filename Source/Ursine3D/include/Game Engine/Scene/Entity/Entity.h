/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Entity.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "EntityConfig.h"
#include "ComponentConfig.h"

#include "TransformComponent.h"

#include "EventDispatcher.h"

#include <string>
#include <type_traits>

namespace ursine
{
    class LocalTimerManager;
    class LocalTweenManager;

    namespace ecs
    {
        // forward declarations
        class World;
        class Component;

        class Entity final
        {
        public:
            typedef uint32 EventID;
            typedef EventDispatcher<EventID> EventDispatcher;

            // Allow use of ChainableEventOperator
            typedef EventID KeyType;

            ////////////////////////////////////////////////////////////////////
            // State/Identification
            ////////////////////////////////////////////////////////////////////

            // Active ID
            EntityID GetID(void) const;

            // Unique ID (basically the total number of entities added to the 
            // world when this entity was created)
            EntityUniqueID GetUniqueID(void) const;

            // determines if the entity is currently being deleted
            bool IsDeleting(void) const;

            // Determines if the entity is active (being used in the world)
            bool IsActive(void) const;

            // Determines if this entity is both active and not being deleted
            bool IsAvailable(void) const;

            // Removes this entity from the world
            void Delete(void);

            // Gets the world this entity belongs to
            World *GetWorld(void) const;

            // Gets the transform for this entity
            Transform *GetTransform(void) const;

            ////////////////////////////////////////////////////////////////////
            // Tags
            ////////////////////////////////////////////////////////////////////

            // Gets the tag for this entity. Empty by default
            const std::string &GetTag(void) const;

            // Sets the tag for this entity.
            void SetTag(const std::string &tag);

            ////////////////////////////////////////////////////////////////////
            // Groups
            ////////////////////////////////////////////////////////////////////

            // Gets groups this entity belongs to
            const EntityGroupVector &GetGroups(void) const;

            // Adds this entity to a specific group
            void AddGroup(const std::string &group);

            // Removes this entity from a specific group
            void RemoveGroup(const std::string &group);

            // Determines if this entity belongs to the specified group
            bool HasGroup(const std::string &group) const;

            ////////////////////////////////////////////////////////////////////
            // Utilities
            ////////////////////////////////////////////////////////////////////

            // Gets the local timer manager for this entity
            LocalTimerManager &GetTimers(void);

            // Gets the local tween manager for this entity
            LocalTweenManager &GetTweens(void);

            ////////////////////////////////////////////////////////////////////
            // Components
            ////////////////////////////////////////////////////////////////////

            // Adds a component this entity. Arguments passed are forwarded to 
            // the components constructor
            template<class ComponentType, typename... Args>
            inline ComponentType *AddComponent(Args&&... args);

            // Removes a component of the specified type (type safe)
            template<class ComponentType>
            inline void RemoveComponent(void);

            // Removes a component of the specified type id
            void RemoveComponent(ComponentTypeID id);

            // Gets a component of the specified type (type safe)
            // nullptr if it doesn't exist
            template<class ComponentType>
            inline ComponentType *GetComponent(void) const;

            // Gets a component of the specified type id
            // nullptr if it doesn't exist. Use the type safe version when possible
            Component *GetComponent(ComponentTypeID id) const;

            // Determines if the entity has this component type (type safe)
            template<class ComponentType>
            inline bool HasComponent(void) const;

            // Determines if the entity has this component type based on 
            // the specified type mask
            bool HasComponent(ComponentTypeMask mask) const;

            // Gets all components for this entity. Avoid this, it's slow.
            // (chances are you don't need to use it)
            ComponentVector GetComponents(void) const;

            // Call a certain member function on all children in the transform
            // hierarchy, and forwards the specified args.
            template<class ComponentType, typename... Args>
            void ApplyChildren(void (ComponentType::*)(Args...), Args&&...);

            ////////////////////////////////////////////////////////////////////
            // Events
            ////////////////////////////////////////////////////////////////////

            template<typename Args>
            void Connect(EventID event, StaticDelegate<Args> delegate);

            template<typename Class, typename Args>
            void Connect(EventID event, Class *context, ClassDelegate<Class, Args> delegate);

            template<typename Args>
            void Disconnect(EventID event, StaticDelegate<Args> delegate);

            template<typename Class, typename Args>
            void Disconnect(EventID event, Class *context, ClassDelegate<Class, Args> delegate);

            void Dispatch(EventID event, const EventArgs *args);
            
            template<typename Listener>
            ChainableEventOperator<Entity, Listener> Listener(Listener *listener = nullptr);

        private:
            enum Flags
            {
                DELETING = 1,
                ACTIVE = 2
            };

            // entity manager needs to be able to construct entities
            friend class EntityManager;

            // access ids directly
            friend class TagManager;
            friend class GroupManager;
            friend class UtilityManager;

            // emplace_back access constructor
            friend class std::allocator<Entity>;

            // access type and system bits
            friend class Filter;

            // access type and system bits
            friend class FilterSystem;

            // misc flags for the state of this entity
            uint32 m_flags;

            // active id in the entity manager
            EntityID m_id;

            // unique id in the entity manager
            EntityUniqueID m_uniqueID;

            // world it's attached to
            World *m_world;

            // pointer to this entities transform
            Transform *m_transform;

            // systems using this entity
            ComponentTypeMask m_systemMask;

            // components attached to this entity
            ComponentTypeMask m_typeMask;

            // disable copy and assign
            Entity(const Entity &rhs) = delete;
            Entity(const Entity &&rhs) = delete;
            Entity &operator=(const Entity &rhs) = delete;

            Entity(World *world, EntityID id);

            // sets/unsets component systems for this entity (which filter systems own it)
            void setSystem(ComponentTypeMask mask);
            void unsetSystem(ComponentTypeMask mask);

            // sets/unsets component types for this entity (which components it has)
            void setType(ComponentTypeMask mask);
            void unsetType(ComponentTypeMask mask);

            // resets data for this entity
            void reset(void);
        };
    }
}

#include "Entity.hpp"