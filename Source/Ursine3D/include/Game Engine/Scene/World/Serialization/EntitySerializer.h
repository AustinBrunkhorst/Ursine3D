/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntitySerializer.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "SerializationConfig.h"

#include "World.h"

namespace ursine
{
    class Json;

    namespace ecs
    {
        class Entity;

        class EntitySerializer
        {
        public:
            EntitySerializer(void);

            Json Serialize(Entity *entity) const;
            Json SerializeArchetype(Entity *entity) const;

            EntityHandle Deserialize(
                World *world, 
                const Json &data, 
                const char *version = kSerializationVersion
            ) const;

            EntityHandle DeserializeArchetype(
                World *world,
                const Json &data
            ) const;

        private:
            friend class WorldSerializer;

            Json serializeComponents(const Entity *entity) const;
            Json serializeChildren(const Entity *entity) const;

            void deserializeInstance(
                Entity *entity, 
                const Json &data, 
                const char *version
            ) const;

            Component *createComponent(const std::string &typeName) const;
            void deserializeComponents(Entity *entity, const Json &data) const;
            void deserializeComponent(Component *component, const Json &data) const;
        };
    }
}