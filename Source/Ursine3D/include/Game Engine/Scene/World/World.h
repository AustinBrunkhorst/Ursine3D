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

#include "WorldEvent.h"

#include "EntityConfig.h"
#include "Filter.h"

#include <vector>
#include <unordered_map>

namespace ursine
{
    namespace ecs
    {
        class Entity;

        class SystemManager;
        class EntityManager;
        class NameManager;
        class UtilityManager;

        class World : public EventDispatcher<WorldEventType>
        {
        public:
            World(void);

            ~World(void);

            Entity *CreateEntity(void);

            // Gets an entity based on its active id
            Entity *GetEntity(EntityID id) const;

            // Gets an entity based its name (first entity with this name)
            Entity *GetEntityFromName(const std::string &name) const;

            // Gets an entity based on its unique id
            Entity *GetEntityUnique(EntityUniqueID uniqueID) const;

            // Gets all entities belonging to a group
            const EntityVector &GetEntitiesFromName(const std::string &group) const;

            // Gets all active entities matching the specified filter
            EntityVector GetEntitiesFromFilter(const Filter &filter) const;

            // Updates the world
            void Update(void);

            // Draws the world
            void Draw(void);

        private:
            friend class Entity;
            friend class WorldSerializer;

            std::vector<Entity*> m_deleted;

            // direct pointers core world managers
            EntityManager *m_entityManager;
            SystemManager *m_systemManager;
            NameManager *m_nameManager;
            UtilityManager *m_utilityManager;

            World(const World &rhs) = delete;

            // adds an entity to the deletion queue
            void deleteEntity(Entity *entity);
        };
    }
}

#include "World.hpp"