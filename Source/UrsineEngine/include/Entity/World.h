/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** World.h
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
#include "Filter.h"

#include "WorldSerializer.h"

#include <vector>
#include <unordered_map>

#include "Camera2D.h"

namespace Ursine
{
    namespace ECS
    {
        class Entity;

        class SystemManager;
        class EntityManager;
        class TagManager;
        class GroupManager;
        class UtilityManager;

        class World
        {
            friend class Entity;
            friend class WorldSerializer;

            std::vector<Entity*> _deleted;

            std::unordered_map<size_t, WorldManager*> _managers;

            // used for constructing/deleting world managers in the proper order
            std::vector<WorldManager*> _manager_queue;

            // direct pointers core world managers
            EntityManager *_entity_manager;
            SystemManager *_system_manager;
            TagManager *_tag_manager;
            GroupManager *_group_manager;
            UtilityManager *_utility_manager;

            // Main camera
            Camera2D _camera;

            WorldSerializer _serializer;

            Json _attributes;

            // determines if the world has already been loaded
            bool _loaded;

            // pointer to the object that owns this world
            void *_owner;

            template<class ManagerType, typename... Args>
            inline ManagerType *addManager(Args&&...);

            // adds an entity to the deletion queue
            void deleteEntity(Entity *entity);

        public:
            World(void);

            ~World(void);

            inline float GetDelta(void) const;

            inline Camera2D &GetCamera(void);

            template<class ManagerType>
            ManagerType *Manager(void);

            // Loads a world from a file
            void Load(const std::string &path);

            // Gets an attribute set in the world file
            const Json &GetAttribute(const std::string &name) const;

            // Gets the owner of this world (as the specified type)
            template<typename OwnerType>
            OwnerType *GetOwner(void);

            // Sets the owner of this world
            void SetOwner(void *owner);

            // Creates an entity without serialization
            Entity *CreateEntity(void);

            // Creates a serialized entity
            Entity *CreateEntity(const std::string &identifier);

            // Creates a serialized entity that merges component data with "merge"
            Entity *CreateEntity(const std::string &identifier, const Json &merge);

            // Gets an entity based on its active id
            Entity *GetEntity(EntityID id) const;

            // Gets an entity based its tag name
            Entity *GetEntity(const std::string &tag) const;

            // Gets an entity based on its unique id
            Entity *GetEntityUnique(EntityUniqueID unique_id) const;

            // Gets all entities belonging to a group
            const EntityVector &GetEntities(const std::string &group) const;

            // Gets all active entities matching the specified filter
            EntityVector GetEntities(const Filter &filter) const;

            // Updates the world
            void Update(void);

            // Draws the world
            void Draw(void);
        };

        ////////////////////////////////////////////////////////////////////////
        // Core Manager Specialization
        ////////////////////////////////////////////////////////////////////////

        extern template
        EntityManager *World::Manager(void);

        extern template
        SystemManager *World::Manager(void);

        extern template
        TagManager *World::Manager(void);

        extern template
        GroupManager *World::Manager(void);

        extern template
        UtilityManager *World::Manager(void);
    }
}

#include "World.hpp"