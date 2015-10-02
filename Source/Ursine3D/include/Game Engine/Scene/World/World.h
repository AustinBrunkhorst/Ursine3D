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
            Meta(DisableNonDynamic)
            World(void);

            ~World(void);

            Meta(Disable)
            Entity *CreateEntity(const std::string &name = "Entity");

            // Gets an entity based on its active id
            Meta(Disable)
            Entity *GetEntity(EntityID id) const;

            // Gets an entity based its name (first entity with this name)
            Meta(Disable)
            Entity *GetEntityFromName(const std::string &name) const;

            // Gets an entity based on its unique id
            Meta(Disable)
            Entity *GetEntityUnique(EntityUniqueID uniqueID) const;

            // Gets all entities belonging to a group
            Meta(Disable)
            const EntityVector &GetEntitiesFromName(const std::string &group) const;

            // Gets all active entities matching the specified filter
            Meta(Disable)
            EntityVector GetEntitiesFromFilter(const Filter &filter) const;

            // Updates the world
            Meta(Disable)
            void Update(void);

            // Renders the world
            Meta(Disable)
            void Render(void);

            SystemManager *GetSystemManager(void);

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
        } Meta(Enable);
    }
}

#include "World.hpp"