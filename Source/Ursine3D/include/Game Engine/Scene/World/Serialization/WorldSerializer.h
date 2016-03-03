/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WorldSerializer.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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
            static Json Serialize(World *world);

            static World *Deserialize(const std::string &filename);
            static World *Deserialize(const Json &data);

            // Merge a new world with an existing one (systems and entities)
            static void MergeDeserialize(const std::string &filename, World *world);
        };
    }
}