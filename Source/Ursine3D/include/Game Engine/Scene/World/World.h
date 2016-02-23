/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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
#include <mutex>

class Project;

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

        typedef uint32 WorldEventID;

        class World : public EventDispatcher<WorldEventID>
        {
        public:
            typedef std::shared_ptr<World> Handle;

            World(void);
            ~World(void);

            Entity *CreateEntity(const std::string &name = "Entity");
            void queueEntityDeletion(Entity *entity);

            // Creates an entity from an archetype file
            Entity *CreateEntityFromArchetype(
                const std::string &filename, 
                const std::string &name = "Entity"
            );

            // Gets an entity based on its active id
            Entity *GetEntity(EntityUniqueID id) const;

            const std::string &GetEntityName(EntityID id) const;

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

            // Updates the world in editor mode
            void EditorUpdate(void);

            // Renders the world
            void Render(void);

            // Renders the world in editor mode
            void EditorRender(void);

            Entity *GetSettings(void) const;

            SystemManager *GetSystemManager(void) const;

            template<typename SystemType>
            SystemType *GetEntitySystem(void);

            Screen *GetOwner(void) const;
            void SetOwner(Screen *owner);

            void DispatchLoad(void);
        private:
            friend class Entity;
            friend class Project;
            friend class WorldSerializer;
            friend class EntitySerializer;

            bool m_loaded;

            std::mutex m_deletionMutex;
            EntityVector m_deleted;

            Entity *m_settings;

            // direct pointers core world managers
            EntityManager *m_entityManager;
            SystemManager *m_systemManager;
            NameManager *m_nameManager;
            UtilityManager *m_utilityManager;

            Screen *m_owner;

            std::unordered_map<std::string, Json> m_archetypeCache;

            World(const World &rhs) = delete;

            // adds an entity to the deletion queue
            void deleteEntity(Entity *entity);
            
            void clearDeletionQueue(void);

            Entity *loadArchetype(const Json &data);
        } Meta(Enable, WhiteListMethods);
    }
}

#include "World.hpp"