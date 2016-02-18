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
            WorldSerializer(void);

            Json Serialize(World *world) const;
            World *Deserialize(const std::string &fileName) const;
            World *Deserialize(const Json &data) const;
        };
    }
}