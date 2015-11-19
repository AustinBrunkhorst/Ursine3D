#pragma once

#include "SerializationConfig.h"

#include "Json.h"

namespace ursine
{
    namespace ecs
    {
        class WorldSerializer
        {
        public:
            WorldSerializer(void);

            Json Serialize(World::Handle world) const;
            World::Handle Deserialize(const std::string &filename) const;
        };
    }
}