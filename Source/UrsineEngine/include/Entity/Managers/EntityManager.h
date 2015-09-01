/* ---------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
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
#include "EntityManagerEvent.h"

#include "EntitySerializer.h"

#include "ComponentRegistrar.h"

#include "WorldManager.h"

#include "EventDispatcher.h"

#include "Entity.h"

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

        class EntityManager final 
            : public WorldManager
            , public EventDispatcher<EntityManagerEvent>
        {
            // can access addComponent (internal method)
            friend class EntitySerializer;

        public:
            explicit EntityManager(World *world);
            ~EntityManager(void);

            // Creates an entity with only a transform
            Entity *Create(void);

            // Creates an entity serialized from "identifier"
            Entity *Create(const std::string &identifier);

            // Creates an entity serialized from "identifier"
            Entity *Create(const std::string &identifier, const Json &merge);

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
            // Events
            ////////////////////////////////////////////////////////////////////

            Entity::EventDispatcher &GetEntityEvents(Entity *entity);

            ////////////////////////////////////////////////////////////////////
            // Serialization
            ////////////////////////////////////////////////////////////////////

            // Serializes all components on an entity to JSON
            Json Serialize(Entity *entity) const;

            // Gets the path to where entity data is stored
            const std::string &GetDataPath(void) const;

            // Sets the path to where entity data is stored
            void SetDataPath(const std::string &path);

            // predicate for comparing components based on unique ID
            static bool CompareComponents(const Component *a, const Component *b);
        private:
            // components by type
            std::array<ComponentVector, kMaxComponentCount> _component_types;

            // inactive entities
            std::vector<Entity *> _inactive;

            // active entities
            std::vector<Entity *> _active;

            // all entities
            std::deque<Entity> _cache;

            // entity event handlers
            std::vector<Entity::EventDispatcher> _events;

            // unique id to entity map
            std::unordered_map<EntityUniqueID, Entity*> _unique;

            // next available entity ID
            EntityID _next_entity_id;

            // next unique entity ID
            EntityUniqueID _next_entity_uid;

            // next unique component unique ID
            ComponentUniqueID _next_component_uid;

            // path to entity data files
            std::string _data_path;

            // serializer that handles entities, archetypes, etc.
            EntitySerializer _serializer;

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