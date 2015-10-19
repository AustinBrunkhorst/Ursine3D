#include "Precompiled.h"

#include "ComponentUtils.h"

#include "Component.h"

using namespace ursine;

JSFunction(GetNativeComponentDatabase)
{
    auto componentType = typeof( ecs::Component );

    auto database = Json::object { };

    for (auto &component : componentType.GetDerivedClasses( ))
    {
        auto &componentName = component.GetName( );

        database[ componentName ] = Json::object {
            { "meta", component.GetMeta( ).SerializeJson( ) },
            { "fields", Json::object { } }
        };

        auto &fieldsObj = database[ componentName ].object_items( ).at( "fields" );

        for (auto &field : component.GetFields( ))
        {
            auto &fieldName = field.GetName( );
            auto fieldType = field.GetType( );

            Json::object enumObj;

            if (fieldType.IsEnum( ))
            {
                auto &handle = fieldType.GetEnum( );
                auto keys = handle.GetKeys( );

                for (auto &key : keys)
                {
                    auto value = handle.GetValue( key );

                    enumObj[ key ] = value.GetType( ).SerializeJson( value );
                }
            }

            auto fieldObj = Json::object {
                { "type", fieldType.GetName( ) },
                { "isEnum", fieldType.IsEnum( ) },
                { "enumValue", enumObj },
                { "meta", field.GetMeta( ).SerializeJson( ) }
            };

            const_cast<Json::object&>( fieldsObj.object_items( ) )
                .insert( std::make_pair( fieldName, fieldObj ) );
        }
    }

    CefRefPtr<CefV8Value> object;

    JsonSerializer::Deserialize( database, object );

    return object;
}