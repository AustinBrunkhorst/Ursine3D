/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ComponentUtils.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ComponentUtils.h"

#include "Component.h"

using namespace ursine;

namespace
{
    void addType(Json::object &types, meta::Type type);
}

JSFunction(GetNativeComponentDatabase)
{
    auto componentType = typeof( ecs::Component );

    auto types = Json::object { };
    auto components = Json::object { };

    for (auto &component : componentType.GetDerivedClasses( ))
    {
        auto componentObj = Json::object { 
            { "meta", component.GetMeta( ).SerializeJson( ) }
        };

        auto fieldsObj = Json::array { };

        for (auto &field : component.GetFields( ))
        {
            auto &fieldName = field.GetName( );
            auto fieldType = field.GetType( );

            addType( types, fieldType );

            fieldsObj.emplace_back( Json::object {
                { "name", fieldName },
                { "type", fieldType.GetName( ) },
                { "meta", field.GetMeta( ).SerializeJson( ) }
            } );
        }

        componentObj[ "fields" ] = fieldsObj;

        components[ component.GetName( ) ] = componentObj;
    }

    CefRefPtr<CefV8Value> object;

    auto database = Json::object {
        { "types", types },
        { "components", components }
    };

    JsonSerializer::Deserialize( database, object );

    return object;
}

Json::array InspectComponentButtons(const meta::Variant &component)
{
    auto methods = component.GetType( ).GetMethods( );

    Json::array inspection;

    for (auto &method : methods)
    {
        auto &meta = method.GetMeta( );

        auto *button = meta.GetProperty<CreateButton>( );

        if (!button)
            continue;

        inspection.emplace_back( Json::object {
            { "name", method.GetName( ) },
            { "text", button->text }
        } );
    }

    return inspection;
}

namespace
{
    void addType(Json::object &types, meta::Type type)
    {
        auto typeName = type.GetName( );

        // already exists
        if (types.find( typeName ) != types.end( ))
            return;

        auto isArray = type.IsArray( );

        Json::object typeObj 
        {
            { "name", typeName },
            { "isArray", isArray }
        };

        if (type.IsEnum( ))
        {
            auto &handle = type.GetEnum( );
            auto keys = handle.GetKeys( );

            Json::object enumObj;

            for (auto &key : keys)
            {
                auto value = handle.GetValue( key );

                enumObj[ key ] = value.SerializeJson( );
            }

            typeObj[ "enumValue" ] = enumObj;
        }
        else
        {
            typeObj[ "enumValue" ] = nullptr;
        }

        Json::array fieldsObj;

        if (!isArray)
        {
            for (auto &field : type.GetFields( ))
            {
                auto fieldType = field.GetType( );
                auto &fieldName = field.GetName( );

                fieldsObj.emplace_back( Json::object {
                    { "name", fieldName },
                    { "type", fieldType.GetName( ) },
                    { "meta", field.GetMeta( ).SerializeJson( ) }
                } );
            }

            typeObj[ "arrayType" ] = nullptr;
        }
        else
        {
            typeObj[ "arrayType" ] = type.GetArrayType( ).GetName( );
        }

        typeObj[ "fields" ] = fieldsObj;

        types[ typeName ] = typeObj;
    }
}