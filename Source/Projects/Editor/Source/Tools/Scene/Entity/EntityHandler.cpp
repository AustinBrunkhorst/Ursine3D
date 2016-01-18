/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityHandler.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "EntityHandler.h"

#include "Editor.h"
#include "Project.h"

#include <Timer.h>
#include <Meta.h>
#include <ComponentConfig.h>

#include <FileSystem.h>
#include <UIFileDialogCallback.h>
#include <EntitySerializer.h>

using namespace ursine;
using namespace std::placeholders;

namespace
{
    void doSaveArchetype(
        ecs::Entity *entity, 
        int selectedFilter, 
        const fs::FileList &files
    )
    {
        if (files.empty( ))
            return;

        auto data = ecs::EntitySerializer( ).SerializeArchetype( entity );
        
        if (!fs::WriteText( files[ 0 ].string( ), data.dump( ) ))
        {
            auto *editor = GetCoreSystem( Editor );

            URSINE_TODO( "Use UI error popup" );
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "Save Error",
                "Unable to save archetype.",
                editor->GetMainWindow( )->GetInternalHandle( )
            );
        }
    }

    meta::Variant editorGetterOverride(const meta::Variant &instance, const meta::Field &field)
    {
        auto &fieldMeta = field.GetMeta( );

        auto *editorGetter = fieldMeta.GetProperty<EditorGetter>( );

        if (editorGetter)
        {
            auto componentType = instance.GetType( );

            auto &getter = componentType.GetMethod( editorGetter->getter );

            UAssert( getter.IsValid( ),
                "Unknown editor getter '%s' on component type '%s'.",
                editorGetter->getter.c_str( ),
                componentType.GetName( ).c_str( )
            );

            // sorry good practice enforcers
            return getter.Invoke( const_cast<meta::Variant&>( instance ) );
        }
        
        return field.GetValue( instance );
    }
}

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

    auto scene = GetCoreSystem( Editor )->GetProject( )->GetScene( );

    if (scene->IsPaused( ))
    {
		// Have this run in the main thread
		Timer::Create(0).Completed([=] {
			m_world->clearDeletionQueue( );
		} );
    }

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
            { "value", type.SerializeJson( instance, editorGetterOverride, false ) }
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

	// Have this run in the main thread
	Timer::Create( 0 ).Completed( [=] {
		auto instance = componentType.CreateDynamic( );
		entity->AddComponent( instance.GetValue<ecs::Component*>( ) );
	} );

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

	// Have this run in the main thread
	Timer::Create(0).Completed([=] {
		entity->RemoveComponent( id );
	} );

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

    auto &fieldMeta = field.GetMeta( );

    auto *editorSetter = fieldMeta.GetProperty<EditorSetter>( );

    auto valueToSet = fieldType.DeserializeJson( value );

    if (editorSetter)
    {
        auto &setter = componentType.GetMethod( editorSetter->setter );

        UAssert( setter.IsValid( ), 
            "Unknown editor setter '%s' on component type '%s'.",
            editorSetter->setter.c_str( ),
            componentType.GetName( ).c_str( )
        );

        meta::Field::SetValue( instance, valueToSet, setter );
    }
    else
    {
        field.SetValue( instance, valueToSet );
    }

    return CefV8Value::CreateUndefined( );
}

JSMethod(EntityHandler::getChildren)
{
    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto &children = *entity->GetChildren( );

    auto childrenArray = CefV8Value::CreateArray( 
        static_cast<int>( children.size( ) ) 
    );

    for (size_t i = 0; i < children.size( ); ++i)
    {
        auto *child = m_world->GetEntity( children[ i ] );

        childrenArray->SetValue( static_cast<int>( i ), 
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

    auto *transform = entity->GetTransform( );

    auto targetParent = arguments[ 0 ];

    // detaching parent
    if (targetParent->IsNull( ))
    {
        auto *currentParent = transform->GetParent( );

        if (currentParent)
            currentParent->RemoveChild( transform );
    }
    else
    {
        auto *targetEntity = 
            m_world->GetEntityUnique( targetParent->GetUIntValue( ) );

        if (!targetEntity)
            return CefV8Value::CreateBool( false );

        targetEntity->GetTransform( )->AddChildAlreadyInLocal( transform );
    }

    return CefV8Value::CreateBool( true );
}

JSMethod(EntityHandler::getSiblingIndex)
{
    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto index = entity->GetTransform( )->GetSiblingIndex( );

    return CefV8Value::CreateUInt( index );
}

JSMethod(EntityHandler::setSiblingIndex)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid arguments.", nullptr );

    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    entity->GetTransform( )->SetSiblingIndex( 
        arguments[ 0 ]->GetUIntValue( ) 
    );

    return CefV8Value::CreateBool( true );
}

JSMethod(EntityHandler::saveAsArchetype)
{
    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto *editor = GetCoreSystem( Editor );

    CefRefPtr<UIFileDialogCallback> callback = 
        new UIFileDialogCallback( std::bind( &doSaveArchetype, entity, _1, _2 ) );

    std::vector<CefString> filters {
        "Archetype Files|.uatype"
    };

    editor->GetMainUI( )->GetBrowser( )->GetHost( )->RunFileDialog(
        static_cast<CefBrowserHost::FileDialogMode>( FILE_DIALOG_SAVE | FILE_DIALOG_OVERWRITEPROMPT_FLAG ),
        "Save Archetype",
        "",
        filters,
        0,
        callback
    );

    return CefV8Value::CreateBool( true );
}

JSMethod(EntityHandler::clone)
{
    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto clone = entity->Clone( );

    return CefV8Value::CreateUInt( clone->GetUniqueID( ) );
}

ecs::Entity *EntityHandler::getEntity(void)
{
    return m_world->GetEntityUnique( m_handle );
}