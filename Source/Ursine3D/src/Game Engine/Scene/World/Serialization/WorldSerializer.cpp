/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WorldSerializer.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "WorldSerializer.h"
#include "EntitySerializer.h"

#include <NameManager.h>

namespace ursine
{
    namespace ecs
    {
        namespace
        {
            const auto kKeySettings = "settings";
            const auto kKeyEntities = "entities";

            const auto kKeyVersion = "v";
            const auto kVersion = "0.0";
        }

        WorldSerializer::WorldSerializer(void)
        {

        }

        Json WorldSerializer::Serialize(World *world) const
        {
            Json::object data;

            data[ kKeyVersion ] = kVersion;

            Json::array entitiesData;

            EntitySerializer entitySerializer;

            auto *settingsEntity = world->GetSettings( );

            data[ kKeySettings ] = 
                entitySerializer.Serialize( settingsEntity );

            auto &rootEntities = *world->m_entityManager
                ->m_hierarchy.GetRootNode( ).Children( );

            for (auto entityID : rootEntities)
            {
                // skip the settings entity
                if (entityID == settingsEntity->m_id)
                    continue;

                auto *entity = world->m_entityManager->GetEntity( entityID );

                // only serialize if enabled
                if (entity->IsSerializationEnabled( ))
                {
                    entitiesData.emplace_back( 
                        entitySerializer.Serialize( entity )
                    );
                }
            }

            data[ kKeyEntities ] = entitiesData;

            return data;
        }

        World *WorldSerializer::Deserialize(const std::string &filename) const
        {
            std::string data;

            if (!fs::LoadAllText( filename, data ))
                throw SerializationException( "Unable to read world file." );

            std::string jsonError;

            auto worldData = Json::parse( data, jsonError );

            if (!jsonError.empty( ))
            {
                std::stringstream error;

                error << "Error parsing world. Error: ";
                error << jsonError;

                throw SerializationException( error.str( ) );
            }

            return Deserialize( worldData );
        }

        World *WorldSerializer::Deserialize(const Json &worldData) const
        {
            auto &versionData = worldData[ kKeyVersion ];

            if (versionData.is_null( ))
            {
                throw SerializationException( 
                    "World missing serialization version." 
                );
            }

            auto version = versionData.string_value( ).c_str( );

            EntitySerializer entitySerializer;

            auto world = new World;

            auto &settingsData = worldData[ kKeySettings ];

            // load settings object if it exists
            if (!settingsData.is_null( ))
            {
                entitySerializer.deserializeInstance(
                    world->GetSettings( ),
                    settingsData,
                    version
                );
            }

            auto &entitiesData = worldData[ kKeyEntities ];

            if (!entitiesData.is_array( ))
            {
                std::stringstream error;

                error << "Expected '";
                error << kKeyEntities; 
                error << "' to be an array.";
                
                throw SerializationException( error.str( ) );
            }

            for (auto &entityData : entitiesData.array_items( ))
            {
                entitySerializer.Deserialize( 
                    world, 
                    entityData, 
                    version 
                );
            }

            world->DispatchLoad( );

            return world;
        }
    }
}