/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GroupManager.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "WorldManager.h"
#include "EntityConfig.h"

#include "GroupManagerEvent.h"

namespace ursine
{
    namespace ecs
    {
        class NameManager final 
            : public WorldManager
            , public EventDispatcher<GroupManagerEvent>
        {
            // group of entities mapped to a specific group name
            std::unordered_map<std::string, EntityVector> m_names;
            
            // groups mapped to a specific entity
            std::unordered_map<EntityUniqueID, EntityGroupVector> m_idMap;

        public:
            NameManager(World *world);
            ~NameManager(void);

            // Adds an entity to a specific group
            void Add(const std::string &name, Entity *entity);

            // Removes an entity from a specific group
            void Remove(const std::string &name, Entity *entity);

            // Removes all groups from this entity
            void Clear(Entity *entity);

            // Gets the first entity with this name
            Entity *GetEntity(const std::string &name);

            // Gets all entities in this group
            const EntityVector &GetEntities(const std::string &name);

            // Gets the name of a specific entity
            const std::string &GetName(const Entity *entity) const;
        };
    }
}