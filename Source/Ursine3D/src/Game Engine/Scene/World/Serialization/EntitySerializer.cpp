/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntitySerializer.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "EntitySerializer.h"

#include "NameManager.h"

namespace ursine
{
    namespace ecs
    {
        namespace
        {
            const auto kKeyEntityName = "name";
            const auto kKeyEntityComponents = "components";
            const auto kKeyEntityChildren = "children";

            const auto kKeyComponentUID = ".uid";

            const auto kKeyArchetypeVersion = "v";

            struct ComponentDeserializationData
            {
                ComponentUniqueID uid;
                Component *component;
                const Json *data;
            };

            bool compareComponentDeserializationData(
                const ComponentDeserializationData &a,
                const ComponentDeserializationData &b
            )
            {
                // note: ascending order
                return a.uid < b.uid;
            }
        }

        EntitySerializer::EntitySerializer(void)
        {
            
        }

        Json EntitySerializer::Serialize(Entity *entity) const
        {
            Json::object data;

            data[ kKeyEntityName ] = entity->GetName( );
            data[ kKeyEntityComponents ] = serializeComponents( entity );
            data[ kKeyEntityChildren ] = serializeChildren( entity );

            return data;
        }

        Json EntitySerializer::SerializeArchetype(Entity *entity) const
        {
            Json::object data;

            data[ kKeyArchetypeVersion ] = kSerializationVersion;
            data[ kKeyEntityComponents ] = serializeComponents( entity );
            data[ kKeyEntityChildren ] = serializeChildren( entity );

            return data;
        }

        Entity *EntitySerializer::Deserialize(
            World *world,
            const Json &data,
            const char *version
        ) const
        {
            auto *entityManager = world->m_entityManager;

            auto *entity = entityManager->create( );

            deserializeInstance( entity, data, version );

            entityManager->dispatchCreated( entity );

            return entity;
        }

        Entity *EntitySerializer::DeserializeArchetype(World *world, const Json &data) const
        {
            const char *version;

            auto &versionData = data[ kKeyArchetypeVersion ];

            if (!versionData.is_string( ))
            {
                UWarning( 
                    "Unknown or missing archetype version, assuming latest."
                );

                version = kSerializationVersion;
            }
            else
            {
                version = versionData.string_value( ).c_str( );
            }

            return Deserialize( world, data, version );
        }

        Json EntitySerializer::serializeComponents(const Entity *entity) const
        {
            Json::object data;

            for (auto *component : entity->GetComponents( ))
            {
                auto componentType = component->GetType( );

                // skip components explicitly disabled
                if (componentType.GetMeta( ).GetProperty<DisableSerialization>( ))
                    continue;

                auto instance = ObjectVariant( component );

                auto serialized = instance.SerializeJson( );

                const_cast<Json::object&>( serialized.object_items( ) )
                    [ kKeyComponentUID ] = Json( static_cast<double>( component->m_uniqueID ) );

                data[ componentType.GetName( ) ] = serialized;
            }

            return data;
        }

        Json EntitySerializer::serializeChildren(const Entity *entity) const
        {
            Json::array data;

            auto *entityManager = entity->m_world->m_entityManager;

            auto &children = *entityManager->m_hierarchy.GetChildren( entity );

            // serialize all children
            for (auto childID : children)
            {
                auto *child = entityManager->GetEntity( childID );

                if (child->IsSerializationEnabled( ))
                    data.emplace_back( Serialize( child ) );
            }

            return data;
        }

        void EntitySerializer::deserializeInstance(
            Entity *entity, 
            const Json &data,
            const char *version
        ) const
        {
            auto &name = data[ kKeyEntityName ];

            if (name.is_string( ))
            {
                entity->m_world->m_nameManager
                    ->setName( entity, name.string_value( ) );
            }

            auto *entityManager = entity->m_world->m_entityManager;

            auto &componentsData = data[ kKeyEntityComponents ];

            deserializeComponents( entity, componentsData );

            entityManager->initializeComponents( entity );

            auto &childrenData = data[ kKeyEntityChildren ];

            if (childrenData.is_array( ))
            {
                for (auto &childData : childrenData.array_items( ))
                {
                    auto *child = entityManager->create( );

					deserializeInstance( child, childData, version );

					entity->m_transform->AddChildAlreadyInLocal( child->m_transform );
                }
            }
        }

        Component *EntitySerializer::createComponent(const std::string &typeName) const
        {
            auto componentType = meta::Type::GetFromName( typeName );

            if (!componentType.IsValid( ))
            {
                std::stringstream error;

                error << "Unknown component type '";
                error << typeName;
                error << "'.";

                throw SerializationException( error.str( ) );
            }

            auto &ctor = componentType.GetDynamicConstructor( );

            if (!ctor.IsValid( ))
            {
                std::stringstream error;

                error << "Component type '";
                error << typeName; 
                error << "' doesn't have dynamic default constructor.";
                
                throw SerializationException( error.str( ) );
            }

            if (componentType.GetMeta( ).GetProperty<DisableSerialization>( ))
            {
                UWarning(
                    "Component type '%s' serialized despite having meta property '%s'.",
                    typeName.c_str( ),
                    typeof( DisableSerialization ).GetName( ).c_str( )
                );
            }

            return ctor.Invoke( ).GetValue<Component*>( );
        }

        void EntitySerializer::deserializeComponents(Entity *entity, const Json &data) const
        {
            if (!data.is_object( ))
            {
                std::stringstream error;

                error << "Expected '";
                error << kKeyEntityComponents;
                error << "' to be an object.";

                throw SerializationException( error.str( ) );
            }

            static const auto &transformName = 
                typeof( ecs::Transform ).GetName( );

            auto &transformData = data[ transformName ];

            // handle transform first, explicitly
            if (transformData.is_null( ))
            {
                // add the transform if it doesn't already exist
                entity->m_transform = new Transform( );
            }
            else
            {
                auto *transform = new Transform( );

                entity->m_world->m_entityManager->addComponent( 
                    entity, 
                    transform 
                );

                deserializeComponent( transform, transformData );

                entity->m_transform = transform;
            }

            std::vector<ComponentDeserializationData> components;

            for (auto &componentData : data.object_items( ))
            {
                // skip transform
                if (componentData.first == transformName)
                    continue;

                ComponentDeserializationData serializationData;

                serializationData.component = 
                    createComponent( componentData.first );

                serializationData.uid = static_cast<ComponentUniqueID>( 
                    componentData.second[ kKeyComponentUID ].number_value( )
                );

                serializationData.data = &componentData.second;

                entity->m_world->m_entityManager->addComponent( 
                    entity, 
                    serializationData.component 
                );

                utils::InsertionSort( 
                    components, 
                    serializationData, 
                    compareComponentDeserializationData 
                );
            }

            for (auto &component : components)
                deserializeComponent( component.component, *component.data );
        }

        void EntitySerializer::deserializeComponent(Component *component, const Json &data) const
        {
            auto instance = ObjectVariant( component );

            instance.GetType( ).DeserializeJson( instance, data );
        }
    }
}