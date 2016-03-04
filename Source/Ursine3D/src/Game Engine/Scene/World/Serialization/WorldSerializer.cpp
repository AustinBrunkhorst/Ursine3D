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

#include "WorldConfigComponent.h"
#include "SystemManager.h"

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

        Json WorldSerializer::Serialize(World *world)
        {
            Json::object data;

            data[ kKeyVersion ] = kVersion;

            Json::array entitiesData;

            EntitySerializer entitySerializer;

            auto &settingsEntity = world->GetSettings( );

            data[ kKeySettings ] = 
                entitySerializer.Serialize( settingsEntity );

            auto &rootEntities = *world->m_entityManager
                ->m_hierarchy.GetRootNode( ).Children( );

            for (auto entityID : rootEntities)
            {
                // skip the settings entity
                if (entityID == settingsEntity->m_id)
                    continue;

                auto entity = world->m_entityManager->GetEntityByID( entityID );

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

        World *WorldSerializer::Deserialize(const std::string &filename)
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

        World *WorldSerializer::Deserialize(const Json &worldData)
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

            return world;
        }

        void WorldSerializer::MergeDeserialize(World::Handle from, World *to)
        {
            EntitySerializer entitySerializer;

            // merge the existing systems in the world config
            auto toSystemManager = to->GetSystemManager( );

            auto &newSettings = from->GetSettings( );
            auto newConfig = newSettings->GetComponent<WorldConfig>( );
            auto &newSystems = newConfig->GetSystems( );

            for (auto &newSystem : newSystems)
            {
                auto systemType = meta::Type::GetFromName( newSystem.type );

                // If the new system isn't found, add it to the current systems
                if (!toSystemManager->HasSystem( systemType ))
                    toSystemManager->AddSystem( systemType );
            }

            // Add the world's entities to the existing world
            auto em = from->GetEntityManager( );
            EntitySerializer serializer;

            auto toAddEntities = em->GetRootEntities( );

            for (auto &entityData : toAddEntities)
            {
                auto data = serializer.Serialize( entityData );

                serializer.Deserialize( to, data );
            }
        }
    }
}