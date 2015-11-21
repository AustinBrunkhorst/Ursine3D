/* ---------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
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

#include "EventDispatcher.h"
#include "WorldEvent.h"

#include "EntityConfig.h"
#include "Filter.h"

#include <vector>

namespace ursine
{
    class Screen;

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
            typedef std::shared_ptr<World> Handle;

            World(void);
            ~World(void);

            Entity *CreateEntity(const std::string &name = "Entity");

            // Gets an entity based on its active id
            Entity *GetEntity(EntityID id) const;

            // Gets an entity based its name (first entity with this name)
            Entity *GetEntityFromName(const std::string &name) const;

            // Gets an entity based on its unique id
            Entity *GetEntityUnique(EntityUniqueID uniqueID) const;

            // Gets entities without parents
            EntityVector GetRootEntities(void) const;

            // Gets all active entities in the world
            const EntityVector &GetActiveEntities(void) const;

            // Gets all entities belonging to a group
            const EntityVector &GetEntitiesFromName(const std::string &group) const;

            // Gets all active entities matching the specified filter
            EntityVector GetEntitiesFromFilter(const Filter &filter) const;

            // Updates the world
            void Update(void);

            // Renders the world
            void Render(void);

            Entity *GetSettings(void) const;

            SystemManager *GetSystemManager(void) const;

            Screen *GetOwner(void) const;
            void SetOwner(Screen *owner);
        private:
            friend class Entity;
            friend class WorldSerializer;
            friend class EntitySerializer;

            EntityVector m_deleted;

            Entity *m_settings;

            // direct pointers core world managers
            EntityManager *m_entityManager;
            SystemManager *m_systemManager;
            NameManager *m_nameManager;
            UtilityManager *m_utilityManager;

            Screen *m_owner;

            World(const World &rhs) = delete;

            void dispatchLoad(void);

            // adds an entity to the deletion queue
            void deleteEntity(Entity *entity);
        } Meta(Enable, WhiteListMethods);
    }
}

#include "World.hpp"