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
        class GroupManager final 
            : public WorldManager
            , public EventDispatcher<GroupManagerEvent>
        {
            // group of entities mapped to a specific group name
            std::unordered_map<std::string, EntityVector> _groups;
            
            // groups mapped to a specific entity
            std::unordered_map<EntityUniqueID, EntityGroupVector> _id_map;

        public:
            GroupManager(World *world);
            ~GroupManager(void);

            // Adds an entity to a specific group
            void Add(const std::string &group, Entity *entity);

            // Removes an entity from a specific group
            void Remove(const std::string &group, Entity *entity);

            // Removes all groups from this entity
            void Clear(Entity *entity);

            // Gets all entities in this group
            const EntityVector &GetEntities(const std::string &group);

            // Gets all groups this entity belongs to
            const EntityGroupVector &GetGroups(const Entity *entity);

            // Determines if the specified entity has the specified group
            bool HasGroup(const std::string &group, const Entity *entity);
        };
    }
}