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
#include "SystemConfig.h"

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

            ////////////////////////////////////////////////////////////////////
            // State/Identification
            ////////////////////////////////////////////////////////////////////

            // Active ID
            EntityID GetID(void) const;

            // Unique ID (basically the total number of entities added to the 
            // world when this entity was created)
            EntityUniqueID GetUniqueID(void) const;

            // Determines if the entity is currently being deleted
            bool IsDeleting(void) const;

            // Determines if the entity is active (being used in the world)
            bool IsActive(void) const;

            // Determines if this entity is both active and not being deleted
            bool IsAvailable(void) const;

            // Determines if this entity can be deleted
            bool IsDeletionEnabled(void) const;

            // Enables deletion for this entity
            void EnableDeletion(bool enabled);

            // Determines if this entity change change it's hierarchy
            bool IsHierarchyChangeEnabled(void) const;

            // Enables hierarchy change for this entity
            void EnableHierarchyChange(bool enabled);

            // Determines if this entity will be serialized
            bool IsSerializationEnabled(void) const;

            // Enables serialization for this entity
            void EnableSerialization(bool enabled);

            // Determines if this entity is visible in the editor
            bool IsVisibleInEditor(void) const;

            // Sets whether or not the entity is visible in the editor
            void SetVisibleInEditor(bool visible);

            // Removes this entity from the world
            void Delete(void);

            // Gets the world this entity belongs to
            World *GetWorld(void) const;

            // Gets the transform for this entity
            Transform *GetTransform(void) const;

            ////////////////////////////////////////////////////////////////////
            // Naming
            ////////////////////////////////////////////////////////////////////

            // Gets the name for this entity
            const std::string &GetName(void) const;

            // Sets the name for this entity
            void SetName(const std::string &name);

            ////////////////////////////////////////////////////////////////////
            // Utilities
            ////////////////////////////////////////////////////////////////////

            // Creates a clone of this entity
            Entity *Clone(void);

            // Gets the local timer manager for this entity
            LocalTimerManager &GetTimers(void);

            // Gets the local tween manager for this entity
            LocalTweenManager &GetTweens(void);

            ////////////////////////////////////////////////////////////////////
            // Components
            ////////////////////////////////////////////////////////////////////

            // Adds a component based on an existing instance
            void AddComponent(Component *component);

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

			////////////////////////////////////////////////////////////////////
			// Hierarchy
			////////////////////////////////////////////////////////////////////

            // Gets children of this entity
            const std::vector<EntityID> *GetChildren(void) const;

			// Gets a component of the specified type in this entity's children (type safe) (depth first)
			// nullptr if it doesn't exist
			template<class ComponentType>
			inline ComponentType *GetComponentInChildren(void) const;

			// Gets a component of the specified type id in this entity's children (depth first)
			// nullptr if it doesn't exist. Use the type safe version when possible
			Component *GetComponentInChildren(ComponentTypeID id) const;

			// Gets a component of the specified type in this entity's parent (type safe)
			// nullptr if it doesn't exist
			template<class ComponentType>
			inline ComponentType *GetComponentInParent(void) const;

			// Gets a component of the specified type id in this entity's parent
			// nullptr if it doesn't exist. Use the type safe version when possible
			Component *GetComponentInParent(ComponentTypeID id) const;

			// Gets the components of the specified type in this entity's children (type safe)
			// nullptr if it doesn't exist
			template<class ComponentType>
			inline std::vector<ComponentType*> GetComponentsInChildren(void) const;

			// Gets the components of the specified type id in this entity's children
			// nullptr if it doesn't exist. Use the type safe version when possible
			ComponentVector GetComponentsInChildren(ComponentTypeID id) const;

			// Gets the components of the specified type in this entity's parents (type safe)
			// nullptr if it doesn't exist
			template<class ComponentType>
			inline std::vector<ComponentType*> GetComponentsInParents(void) const;

			// Gets the components of the specified type id in this entity's parents
			// nullptr if it doesn't exist. Use the type safe version when possible
			ComponentVector GetComponentsInParents(ComponentTypeID id) const;

            // Find this entity's index in relation to the other children
            uint GetSiblingIndex(void) const;

            // Move this entity to the start of the parent's children lists
            void SetAsFirstSibling(void);

            // Sets this entity's index in the parent's children list
            void SetSiblingIndex(uint index) const;

            ////////////////////////////////////////////////////////////////////
            // Events
            ////////////////////////////////////////////////////////////////////

            template<typename Args>
            void Connect(
                EventID event, 
                EventDispatcher::HandlerType::StaticDelegate<Args> delegate,
                EventHandlerPriority priority = kDefaultEventHandlerPriority
            );

            template<typename Class, typename Args>
            void Connect(
                EventID event, 
                Class *context, 
                EventDispatcher::HandlerType::ClassDelegate<Class, Args> delegate,
                EventHandlerPriority priority = kDefaultEventHandlerPriority
            );

            template<typename Args>
            void Disconnect(EventID event, EventDispatcher::HandlerType::StaticDelegate<Args> delegate);

            template<typename Class, typename Args>
            void Disconnect(EventID event, Class *context, EventDispatcher::HandlerType::ClassDelegate<Class, Args> delegate);

            void Dispatch(EventID event, const EventArgs *args);
            
            template<typename Listener>
            ChainableEventOperator<Entity, Listener> Listener(Listener *listener = nullptr);

        private:
            // entity manager needs to be able to construct entities
            friend class EntityManager;
            friend class WorldSerializer;
            friend class EntitySerializer;

            // access ids directly
            friend class NameManager;
            friend class UtilityManager;

            // emplace_back access constructor
            friend class std::allocator<Entity>;

            // access type and system bits
            friend class Filter;

            // access type and system bits
            friend class FilterSystem;

            bool m_active                 : 1;
            bool m_deleting               : 1;
            bool m_deletionEnabled        : 1;
            bool m_hierarchyChangeEnabled : 1;
            bool m_serializationEnabled   : 1;
            bool m_visibleInEditor        : 1;

            // active id in the entity manager
            EntityID m_id;

            // unique id in the entity manager
            EntityUniqueID m_uniqueID;

            // world it's attached to
            World *m_world;

            // pointer to this entities transform
            Transform *m_transform;

            // systems using this entity
            SystemTypeMask m_systemMask;

            // components attached to this entity
            ComponentTypeMask m_typeMask;

            // disable copy and assign
            Entity(const Entity &rhs) = delete;
            Entity(const Entity &&rhs) = delete;
            Entity &operator=(const Entity &rhs) = delete;

            Entity(World *world, EntityID id);

            // sets/unsets component systems for this entity (which filter systems own it)
            void setSystem(SystemTypeMask mask);
            void unsetSystem(SystemTypeMask mask);

            // sets/unsets component types for this entity (which components it has)
            void setType(ComponentTypeMask mask);
            void unsetType(ComponentTypeMask mask);

            // resets data for this entity
            void reset(void);

			// set the entity and all of it's children's deleting flags
			void setDeletingTrue(void);
        };
    }
}

#include "Entity.hpp"