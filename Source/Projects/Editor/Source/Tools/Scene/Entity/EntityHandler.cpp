#include "Precompiled.h"

#include "EntityHandler.h"

#include "Editor.h"
#include "Project.h"

#include <Meta.h>
#include <ComponentConfig.h>

using namespace ursine;

JSConstructor(EntityHandler)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid constructor arguments." );

    m_handle = arguments[ 0 ]->GetUIntValue( );
    m_world = &GetCoreSystem( Editor )->GetProject( )->GetScene( ).GetWorld( );
}

JSMethod(EntityHandler::isValid)
{
    return CefV8Value::CreateBool( getEntity( ) != nullptr );
}

JSMethod(EntityHandler::getName)
{
    auto entity = getEntity( );

    if (entity)
        return CefV8Value::CreateString( entity->GetName( ) );

    return CefV8Value::CreateBool( false );
}

JSMethod(EntityHandler::inspect)
{
    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto components = entity->GetComponents( );

    Json componentArray = Json::array { };

    auto &items = const_cast<Json::array&>( componentArray.array_items( ) );

    for (auto *component : components)
    {
        meta::Variant instance { component, meta::variant_policy::WrapObject( ) };

        auto type = instance.GetType( );

        items.emplace_back( Json::object {
            { "type", type.GetName( ) },
            { "value", type.SerializeJson( instance ) }
        } );
    }

    CefRefPtr<CefV8Value> output;

    JsonSerializer::Deserialize( componentArray, output );

    return output;
}

JSMethod(EntityHandler::updateComponentField)
{
    if (arguments.size( ) != 3)
        JSThrow( "Invalid arguments.", nullptr );

    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto componentName = arguments[ 0 ]->GetStringValue( ).ToString( );
    auto fieldName = arguments[ 1 ]->GetStringValue( ).ToString( );

    auto componentType = meta::Type::GetFromName( componentName );

    if (!componentType.IsValid( ))
        JSThrow( "Unknown component type.", nullptr );

    auto &componentID = componentType.GetStaticField( "ComponentID" );

    if (!componentID.IsValid( ))
        JSThrow( "Invalid component type.", nullptr );

    auto *component = entity->GetComponent( 
        componentID.GetValue( ).GetValue<ecs::ComponentTypeID>( ) 
    );

    auto value = JsonSerializer::Serialize( arguments[ 2 ] );

    meta::Variant instance { component, meta::variant_policy::WrapObject( ) };

    auto field = componentType.GetField( fieldName );

    if (!field.IsValid( ))
        JSThrow( "Invalid field.", nullptr );

    auto fieldType = field.GetType( );

    field.SetValue( instance, fieldType.DeserializeJson( value ) );

    return CefV8Value::CreateUndefined( );
}

ecs::Entity *EntityHandler::getEntity(void)
{
    return m_world->GetEntityUnique( m_handle );
}