/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

namespace ursine
{
    namespace ecs
    {
        class NameManager final : public WorldManager
        {
        public:
            NameManager(World *world);
            ~NameManager(void);

            // Removes this entity from reference
            void Remove(Entity *entity);

            // Gets the first entity with this name
            Entity *GetEntity(const std::string &name);

            // Gets all entities in this group
            const EntityVector &GetEntities(const std::string &name);

            // Gets the name of a specific entity
            const std::string &GetName(const Entity *entity);
            void SetName(Entity *entity, const std::string &name);

        private:
            friend class EntitySerializer;

            // group of entities mapped to a specifc name
            std::unordered_map<std::string, EntityVector> m_grouped;

            // entities mapped to specific names
            std::unordered_map<EntityUniqueID, std::string> m_names;

            void setName(Entity *entity, const std::string &name);
            void removeFromGroup(const std::string &name, Entity *entity);
        };
    }
}