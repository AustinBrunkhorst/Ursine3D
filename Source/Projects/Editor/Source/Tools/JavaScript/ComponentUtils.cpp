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

            auto fieldObj = Json::object {
                { "type", field.GetType( ).GetName( ) },
                { "meta", field.GetMeta( ).SerializeJson( ) }
            };

            const_cast<Json::object&>( fieldsObj.object_items( ) )
                .insert( std::make_pair( fieldName, fieldObj ) );
        }
    }

    CefRefPtr<CefV8Value> object;

    Json test = database;

    auto str = test.dump( );

    JsonSerializer::Deserialize( database, object );

    return object;
}