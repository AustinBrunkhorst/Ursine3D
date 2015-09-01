/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WorldSerializer.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


#include "UrsinePrecompiled.h"

#include "WorldSerializer.h"

#include "World.h"

#include "WorldConfig.h"
#include "EntityConfig.h"

namespace ursine
{
    namespace ecs
    {
        Json WorldSerializer::Serialize(World *entity) const
        {
            return Json();
        }

        void WorldSerializer::Deserialize(const std::string &identifier, World *world)
        { 
            auto data = loadIdentifier(identifier);

            auto &systems = data[kWorldKeySystems];

            UAssert(systems.is_array() || systems.is_null(),
                "World systems must be an array if defined");

            auto system_manager = world->Manager<SystemManager>();

            for (auto &system : systems.array_items())
                system_manager->AddSystem( system.string_value() );

            world->_attributes = data[kWorldKeyAttributes];

            UAssert(world->_attributes.is_object() || world->_attributes.is_null(), 
                "World attributes must be an object if defined");

            auto entities = data[kWorldKeyEntities];

            if (entities.is_array())
            {
                auto size = entities.array_items().size();

                for (uint32 i = 0; i < size; ++i)
                {
                    auto &entity = entities[i];

                    auto &archetype = entity[kEntityKeyType].string_value();

                    // are we merging components?
                    if (entity[kEntityKeyComponents].is_array())
                        world->CreateEntity(archetype, entity);
                    else
                        world->CreateEntity(archetype);
                }
            }
        }

        Json WorldSerializer::loadIdentifier(const std::string &identifier)
        {
            auto text = gContentManager->LoadText(identifier);

            std::string error;

            auto json = Json::parse(*text, error);

            UAssert(error.empty(),
                "Error parsing World JSON (%s)\n%s",
                identifier.c_str(), error.c_str());

            return json;
        }
    }
}