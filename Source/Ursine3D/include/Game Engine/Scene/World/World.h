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

namespace ursine
{
    class Scene;

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

            EntityHandle CreateEntity(const std::string &name = "Entity");

            // Creates an entity from an archetype file
            EntityHandle CreateEntityFromArchetype(
                const std::string &fileName,
                const std::string &name = "Entity"
            );

            // Creates an entity from an archetype resource
            EntityHandle CreateEntityFromArchetype(
                const resources::ResourceReference &resource
            );

            // Gets an entity based on its id
            EntityHandle GetEntity(EntityID id) const;

            // Gets an entity based its name (first entity with this name)
            EntityHandle GetEntityFromName(const std::string &name) const;

            // Gets entities without parents
            EntityHandleVector GetRootEntities(void) const;

            // Gets all active entities in the world
            EntityHandleVector GetActiveEntities(void) const;

            // Gets all entities belonging to a group
            EntityHandleVector GetEntitiesFromName(const std::string &group) const;

            // Gets all active entities matching the specified filter
            EntityHandleVector GetEntitiesFromFilter(const Filter &filter) const;

            // Updates the world
            void Update(void);

            // Updates the world in editor mode
            void EditorUpdate(void);

            // Renders the world
            void Render(void);

            // Renders the world in editor mode
            void EditorRender(void);

            const EntityHandle &GetSettings(void) const;

            EntityManager *GetEntityManager(void) const;
            SystemManager *GetSystemManager(void) const;

            template<typename SystemType>
            SystemType *GetEntitySystem(void);

            Scene *GetOwner(void) const;
            void SetOwner(Scene *owner);

            void MessageUI(const std::string &message, const Json &data);

            void DispatchLoad(void);

            void MergeWorld(
                resources::ResourceManager &resourceManager, 
                resources::ResourceReference &worldResource
            );

        private:
            friend class Entity;
            friend class EntityHandle;
            friend class WorldSerializer;
            friend class EntitySerializer;
            friend class Scene;

            std::mutex m_deletionMutex;
            EntityHandleVector m_deleted;

            EntityHandle m_settings;

            // direct pointers core world managers
            EntityManager *m_entityManager;
            SystemManager *m_systemManager;
            NameManager *m_nameManager;
            UtilityManager *m_utilityManager;

            Scene *m_owner;

            std::unordered_map<std::string, Json> m_archetypeCache;

            World(const World &rhs) = delete;

            void setOwner(Scene *owner);

            void queueEntityDeletion(Entity *entity);

            // adds an entity to the deletion queue
            void deleteEntity(const EntityHandle &entity);
            
            void clearDeletionQueue(void);

            EntityHandle loadArchetype(const Json &data);
        } Meta(Enable, EnablePtrType, WhiteListMethods);
    }
}

#include "World.hpp"