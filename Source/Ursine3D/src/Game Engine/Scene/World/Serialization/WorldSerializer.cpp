#include "UrsinePrecompiled.h"

#include "WorldSerializer.h"

#include <NameManager.h>

namespace
{
    const auto kKeySettings = "settings";
    const auto kKeyEntities = "entities";
    
    const auto kKeyEntityName = "name";
    const auto kKeyEntityComponents = "components";

    const auto kKeyVersion = "v";
    const auto kVersion = "0.0";
}

namespace ursine
{
    namespace ecs
    {
        WorldSerializer::WorldSerializer(void)
        {

        }

        Json WorldSerializer::Serialize(World::Handle world)
        {
            Json::object data;

            data[ kKeyVersion ] = kVersion;

            Json::array entitiesData;

            auto *settingsEntity = world->GetSettings( );

            data[ kKeySettings ] = serializeComponents( settingsEntity );

            for (auto *entity : world->m_entityManager->GetActiveEntities( ))
            {
                // skip the settings entity or hidden entities
                if (entity->GetUniqueID( ) == settingsEntity->GetUniqueID( ) || 
                    !entity->IsVisibleInEditor( ))
                {
                    continue;
                }

                entitiesData.emplace_back( 
                    serializeEntity( entity ) 
                );
            }

            data[ kKeyEntities ] = entitiesData;

            return data;
        }

        bool WorldSerializer::Deserialize(const std::string &filename, World::Handle &out)
        {
            std::string data;

            if (!fs::LoadText( filename, data ))
            {
                UWarning( "Unable to read world.\nfile: %s", 
                    filename.c_str( )
                );

                return false;
            }

            std::string jsonError;

            auto worldData = Json::parse( data, jsonError );

            if (!jsonError.empty( ))
            {
                UWarning( "Error parsing world.\nfile: %s\nerror: %s", 
                    filename.c_str( ), 
                    jsonError.c_str( ) 
                );

                return false;
            }

            URSINE_TODO( "Replace with exceptions and account for errors to handle potential memory leak" );
            auto world = std::make_shared<World>( );

            auto *entityManager = world->m_entityManager;
            auto *nameManager = world->m_nameManager;

            auto &settingsData = worldData[ kKeySettings ];

            // load settings object if it exists
            if (!settingsData.is_null( ))
            {
                if (!deserializeComponents( 
                        entityManager, 
                        world->GetSettings( ), 
                        settingsData 
                    ))
                    return false;
            }

            auto &entitiesData = worldData[ kKeyEntities ];

            if (!entitiesData.is_array( ))
            {
                UWarning( "Expected '%s' to be an array.\nfile: %s", 
                    kKeyEntities, 
                    filename.c_str( ) 
                );

                return false;
            }

            for (auto &entityData : entitiesData.array_items( ))
            {
                auto *entity = entityManager->create( );

                auto &name = entityData[ kKeyEntityName ];

                if (name.is_string( ))
                    nameManager->setName( entity, name.string_value( ) );

                auto &componentsData = entityData[ kKeyEntityComponents ];

                if (!deserializeComponents( entityManager, entity, componentsData ))
                    return false;

                entityManager->initializeComponents( entity );
                entityManager->dispatchCreated( entity );
            }

            world->DispatchLoad( );

            out = world;

            return true;
        }

        Json WorldSerializer::serializeEntity(const Entity *entity)
        {
            Json::object data;

            data[ kKeyEntityName ] = entity->GetName( );
            data[ kKeyEntityComponents ] = serializeComponents( entity );

            return data;
        }

        Json WorldSerializer::serializeComponents(const Entity *entity)
        {
            Json::object data;

            for (auto *component : entity->GetComponents( ))
            {
                auto componentType = component->GetType( );

                // skip components hidden in inspector
                /*if (componentType.GetMeta( ).GetProperty<HiddenInInspector>( ))
                    continue;*/

                auto instance = meta::Variant { 
                    component, 
                    meta::variant_policy::WrapObject( ) 
                };

                data[ componentType.GetName( ) ] = instance.SerializeJson( );
            }

            return data;
        }

        bool WorldSerializer::deserializeComponents(EntityManager *manager, Entity *entity, const Json &data)
        {
            if (!data.is_object( ))
            {
                UWarning( "Expected '%s' to be an object.", 
                    kKeyEntityComponents
                );

                return false;
            }

            static const auto &transformName = typeof( ecs::Transform ).GetName( );

            auto &transformData = data[ transformName ];

            // handle transform first, explicitly
            if (transformData.is_null( ))
            {
                // add the transform if it doesn't already exist
                entity->m_transform = new Transform( );
            }
            else
            {
                Component *transform;

                if (!deserializeComponent( entity, transformName, transformData, transform ))
                    return false;

                entity->m_transform = static_cast<Transform*>( transform );

                manager->addComponent( entity, transform );
            }

            for (auto &componentData : data.object_items( ))
            {
                // skip transform
                if (componentData.first == transformName)
                    continue;

                Component *component;

                if (!deserializeComponent( entity, componentData.first, componentData.second, component ))
                    return false;

                manager->addComponent( entity, component );
            }
            
            return true;
        }

        bool WorldSerializer::deserializeComponent(Entity *entity, const std::string &typeName, const Json &data, Component *&out)
        {
            auto componentType = meta::Type::GetFromName( typeName );

            if (!componentType.IsValid( ))
            {
                UWarning( "Unknown component type '%s'.", 
                    typeName.c_str( )
                );

                return false;
            }

            auto ctor = componentType.GetDynamicConstructor( );

            if (!ctor.IsValid( ))
            {
                UWarning( "Component type '%s' doesn't have dynamic default constructor.", 
                    componentType.GetName( ).c_str( )
                );

                return false;
            }

            URSINE_TODO( "find better solution to this" );
            auto instance = meta::Variant { 
                ctor.Invoke( ).GetValue<Component*>( ), 
                meta::variant_policy::WrapObject( ) 
            };

            auto &component = instance.GetValue<Component>( );

            component.m_owner = entity;

            componentType.DeserializeJson( instance, data );

            out = &component;
        }
    }
}