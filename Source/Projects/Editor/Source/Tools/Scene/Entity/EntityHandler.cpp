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
    m_world = GetCoreSystem( Editor )->GetProject( )->GetScene( )->GetWorld( ).get( );
}

JSMethod(EntityHandler::isValid)
{
    return CefV8Value::CreateBool( getEntity( ) != nullptr );
}

JSMethod(EntityHandler::isRemovalEnabled)
{
    auto entity = getEntity( );

    return CefV8Value::CreateBool( entity && entity->IsDeletionEnabled( ) );
}

JSMethod(EntityHandler::isHierarchyChangeEnabled)
{
    auto entity = getEntity( );

    return CefV8Value::CreateBool( entity && entity->IsHierarchyChangeEnabled( ) );
}

JSMethod(EntityHandler::isVisibleInEditor)
{
    auto entity = getEntity( );

    return CefV8Value::CreateBool( entity && entity->IsVisibleInEditor( ) );
}

JSMethod(EntityHandler::remove)
{
    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    entity->Delete( );

    return CefV8Value::CreateBool( true );
}

JSMethod(EntityHandler::getName)
{
    auto entity = getEntity( );

    if (entity)
        return CefV8Value::CreateString( entity->GetName( ) );

    return CefV8Value::CreateBool( false );
}

JSMethod(EntityHandler::setName)
{
    auto entity = getEntity( );

    if (entity)
    {
        entity->SetName( arguments[ 0 ]->GetStringValue( ) );

        return CefV8Value::CreateBool( true );
    }

    return CefV8Value::CreateBool( false );
}

JSMethod(EntityHandler::inspect)
{
    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto components = entity->GetComponents( );

    Json::array componentArray;

    for (auto *component : components)
    {
        meta::Variant instance { component, meta::variant_policy::WrapObject( ) };

        auto type = instance.GetType( );

        componentArray.emplace_back( Json::object {
            { "type", type.GetName( ) },
            { "value", type.SerializeJson( instance ) }
        } );
    }

    CefRefPtr<CefV8Value> output;

    JsonSerializer::Deserialize( componentArray, output );

    return output;
}

JSMethod(EntityHandler::hasComponent)
{
    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto componentName = arguments[ 0 ]->GetStringValue( ).ToString( );

    auto componentType = meta::Type::GetFromName( componentName );

    if (!componentType.IsValid( ))
        return CefV8Value::CreateBool( false );

    auto &componentID = componentType.GetStaticField( "ComponentID" );

    if (!componentID.IsValid( ))
        return CefV8Value::CreateBool( false );

    auto id = componentID.GetValue( ).GetValue<ecs::ComponentTypeID>( );

    return CefV8Value::CreateBool( entity->HasComponent( 1ull << id ) );
}

JSMethod(EntityHandler::addComponent)
{
    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto componentName = arguments[ 0 ]->GetStringValue( ).ToString( );

    auto componentType = meta::Type::GetFromName( componentName );

    if (!componentType.IsValid( ))
        return CefV8Value::CreateBool( false );

    auto instance = componentType.CreateDynamic( );

    entity->AddComponent( instance.GetValue<ecs::Component*>( ) );

    return CefV8Value::CreateBool( true );
}

JSMethod(EntityHandler::removeComponent)
{
    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto componentName = arguments[ 0 ]->GetStringValue( ).ToString( );

    auto componentType = meta::Type::GetFromName( componentName );

    if (!componentType.IsValid( ))
        return CefV8Value::CreateBool( false );

    auto &componentID = componentType.GetStaticField( "ComponentID" );

    if (!componentID.IsValid( ))
        return CefV8Value::CreateBool( false );

    auto id = componentID.GetValue( ).GetValue<ecs::ComponentTypeID>( );

    entity->RemoveComponent( id );

    return CefV8Value::CreateBool( true );
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

JSMethod(EntityHandler::getChildren)
{
    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto &children = *entity->GetChildren( );

    auto childrenArray = CefV8Value::CreateArray( children.size( ) );

    for (size_t i = 0; i < children.size( ); ++i)
    {
        auto *child = m_world->GetEntity( children[ i ] );

        childrenArray->SetValue( i, 
            CefV8Value::CreateUInt( child->GetUniqueID( ) ) 
        );
    }

    return childrenArray;
}

JSMethod(EntityHandler::getParent)
{
    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto *parent = entity->GetTransform( )->GetParent( );

    if (!parent)
        return CefV8Value::CreateNull( );

    return CefV8Value::CreateUInt( parent->GetOwner( )->GetUniqueID( ) );
}

JSMethod(EntityHandler::setParent)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid arguments.", nullptr );

    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto *parent = m_world->GetEntityUnique( arguments[ 0 ]->GetUIntValue( ) );

    if (!parent)
        return CefV8Value::CreateBool( false );

    parent->GetTransform( )->AddChildAlreadyInLocal( entity->GetTransform( ) );

    return CefV8Value::CreateBool( true );
}

ecs::Entity *EntityHandler::getEntity(void)
{
    return m_world->GetEntityUnique( m_handle );
}