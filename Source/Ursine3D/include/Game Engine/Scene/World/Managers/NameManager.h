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
            void Remove(EntityID id);

            // Gets the first entity with this name
            EntityHandle GetEntity(const std::string &name) const;

            // Gets all entities in this group
            EntityHandleVector GetEntities(const std::string &name) const;

            // Gets the name of a specific entity
            const std::string &GetName(EntityID id);
            void SetName(EntityID id, const std::string &name);

        private:
            friend class EntitySerializer;

            // group of entities mapped to a specifc name
            std::unordered_map<std::string, EntityIDVector> m_grouped;

            // entities mapped to specific names
            std::unordered_map<EntityID, std::string> m_names;

            void setName(EntityID id, const std::string &name);
            void removeFromGroup(const std::string &name, EntityID id);
        };
    }
}