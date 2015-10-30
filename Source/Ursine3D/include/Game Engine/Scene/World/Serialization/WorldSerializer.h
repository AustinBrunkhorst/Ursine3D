#pragma once

#include "FileSystem.h"
#include "Json.h"

namespace ursine
{
    namespace ecs
    {
        class WorldSerializer
        {
        public:
            WorldSerializer(void);

            Json Serialize(World::Handle world);
            bool Deserialize(const std::string &filename, World::Handle &out);
        private:
            Json serializeEntity(const Entity *entity);
            Json serializeComponents(const Entity *entity);

            bool deserializeComponents(EntityManager *manager, Entity *entity, const Json &data);
        };
    }
}