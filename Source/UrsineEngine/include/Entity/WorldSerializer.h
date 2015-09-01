/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WorldSerializer.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    class Json;

    namespace ecs
    {
        class World;

        class WorldSerializer final
        {
            Json loadIdentifier(const std::string &identifier);

        public:
            Json Serialize(World *entity) const;

            void Deserialize(const std::string &identifier, World *world);
        };
    }
}