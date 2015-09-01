/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TagManager.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "WorldManager.h"
#include "TagManagerEvent.h"

#include "EventDispatcher.h"

#include <unordered_map>

namespace ursine
{
    namespace ecs
    {
        class Entity;

        class TagManager final 
            : public WorldManager
            , public EventDispatcher<TagManagerEvent>
        {
            // tags mapped to entities
            std::unordered_map<std::string, Entity *> _tags;

            // entity ids mapped to tags
            std::unordered_map<EntityUniqueID, std::string> _id_map;

        public:
            TagManager(World *world);
            ~TagManager(void);

            // Gets the tag for a specified entity
            const std::string &GetTag(const Entity *entity);

            // Gets an entity with the specified tag
            Entity *GetEntity(const std::string &tag) const;

            // Registers an entity with this tag
            void Register(const std::string &tag, Entity *entity);

            // UnRegisters an entities tag
            void UnRegister(Entity *entity);
        };
    }
}