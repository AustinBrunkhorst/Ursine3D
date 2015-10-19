/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityManager.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "EntityConfig.h"

#include "WorldManager.h"

#include "Entity.h"

#include "Hierarchy.h"

#include <unordered_map>
#include <vector>
#include <array>
#include <deque>

namespace ursine
{
    namespace ecs
    {
        // forward declarations
        class Component;
        class World;
        class Filter;

        class EntityManager final : public WorldManager
        {
        public:
            explicit EntityManager(World *world);
            ~EntityManager(void);

            // Creates an entity with only a transform
            Entity *Create(void);

            // Gets all entities who match this filter
            EntityVector GetEntities(const Filter &aspect) const;

            // Finds an entity with the given id
            Entity *GetEntity(EntityID id);

            // Finds an entity with the given unique id
            Entity *GetEntityUnique(EntityUniqueID id) const;

            // Called right when an entity is requested to be removed
            void BeforeRemove(Entity *entity);

            // Removes the given entity from the world
            void Remove(Entity *entity);

            ////////////////////////////////////////////////////////////////////
            // Components
            ////////////////////////////////////////////////////////////////////

            // Adds a component to the entity [type safe]
            template<class ComponentType>
            void AddComponent(Entity *entity, ComponentType *component);

            // Adds a component to the entity [non type safe]
            void AddComponent(Entity *entity, Component *component);

            // Removes a component from the entity [type safe]
            template<class ComponentType>
            void RemoveComponent(Entity *entity);

            // Removes a component from the entity [non type safe]
            void RemoveComponent(Entity *entity, ComponentTypeID id);

            // Gets an entity's component of the specified type (type safe)
            // nullptr if it doesn't exist.
            template<class ComponentType>
            inline ComponentType *GetComponent(const Entity *entity) const;

            // Gets an entity's component of the specified type id
            // nullptr if it doesn't exist. Use the type safe version when possible
            Component *GetComponent(const Entity *entity, ComponentTypeID id) const;

            // Gets all components for this entity
            ComponentVector GetComponents(const Entity *entity) const;

            ////////////////////////////////////////////////////////////////////
            // Hierarchy
            ////////////////////////////////////////////////////////////////////

            // Gets a component of the specified type in this entity's children (type safe) (depth first)
            // nullptr if it doesn't exist
            template<class ComponentType>
            inline ComponentType *GetComponentInChildren(const Entity *entity) const;

            // Gets a component of the specified type id in this entity's children (depth first)
            // nullptr if it doesn't exist. Use the type safe version when possible
            Component *GetComponentInChildren(const Entity *entity, ComponentTypeID id) const;

            // Gets a component of the specified type in this entity's parent (type safe)
            // nullptr if it doesn't exist
            template<class ComponentType>
            inline ComponentType *GetComponentInParent(const Entity *entity) const;

            // Gets a component of the specified type id in this entity's parent
            // nullptr if it doesn't exist. Use the type safe version when possible
            Component *GetComponentInParent(const Entity *entity, ComponentTypeID id) const;

            // Gets the components of the specified type in this entity's children (type safe)
            // nullptr if it doesn't exist
            template<class ComponentType>
            inline std::vector<ComponentType*> GetComponentsInChildren(const Entity *entity) const;

            // Gets the components of the specified type id in this entity's children
            // nullptr if it doesn't exist. Use the type safe version when possible
            ComponentVector GetComponentsInChildren(const Entity *entity, ComponentTypeID id) const;

            // Gets the components of the specified type in this entity's parents (type safe)
            // nullptr if it doesn't exist
            template<class ComponentType>
            inline std::vector<ComponentType*> GetComponentsInParents(const Entity *entity) const;

            // Gets the components of the specified type id in this entity's parents
            // nullptr if it doesn't exist. Use the type safe version when possible
            ComponentVector GetComponentsInParents(const Entity *entity, ComponentTypeID id) const;

            // Find the entity's index in relation to the other children
            uint GetSiblingIndex(const Entity *entity) const;

            // Move the entity to the start of the parent's children lists
            void SetAsFirstSibling(const Entity *entity);

            // Sets the entity's index in the parent's children list
            void SetSiblingIndex(const Entity *entity, uint index);

            ////////////////////////////////////////////////////////////////////
            // Events
            ////////////////////////////////////////////////////////////////////

            Entity::EventDispatcher &GetEntityEvents(Entity *entity);

            ////////////////////////////////////////////////////////////////////
            // Serialization
            ////////////////////////////////////////////////////////////////////

            // predicate for comparing components based on unique ID
            static bool CompareComponents(const Component *a, const Component *b);

        private:
            // components by type
            std::array<ComponentVector, kMaxComponentCount> m_componentTypes;

            // inactive entities
            std::vector<Entity *> m_inactive;

            // active entities
            std::vector<Entity *> m_active;

            // all entities
            std::deque<Entity> m_cache;

            // entity event handlers
            std::vector<Entity::EventDispatcher> m_events;

            // unique id to entity map
            std::unordered_map<EntityUniqueID, Entity*> m_unique;

            // next available entity ID
            EntityID m_nextEntityID;

            // next unique entity ID
            EntityUniqueID m_nextEntityUID;

            // next unique component unique ID
            ComponentUniqueID m_nextComponentUID;

            // the hierarchy that all entities reside in
            Hierarchy m_hierarchy;

            // creates an empty entity and adds it to the world
            Entity *create(void);

            // dispatches the entity creation event with this entity
            void dispatchCreated(Entity *entity);

            // internal method for adding a component to the entity
            void addComponent(Entity *entity, Component *component);

            // internal method for removing a component from an entity
            // returns true if the component was removed
            void removeComponent(Entity *entity, ComponentTypeID id, bool dispatch = false);

            // initializes all components after the Transform
            void initializeComponents(Entity *entity);

            // removes and deconstructs all components from this entity
            void clearComponents(Entity *entity, bool dispatch = false);
        };
    }
}

#include "EntityManager.hpp"