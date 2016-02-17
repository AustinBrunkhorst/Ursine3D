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

#include "ComponentUtils.h"

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
    void doOpenErrorLog(Notification &notification)
    {
        utils::OpenPath( URSINE_ERROR_LOG_FILE );
    }

    void doSaveArchetype(
        ecs::Entity *entity, 
        int selectedFilter, 
        const fs::FileList &files
    )
    {
        if (files.empty( ))
            return;

        auto data = ecs::EntitySerializer( ).SerializeArchetype( entity );
        
        auto file = files[ 0 ].string( );

        if (!fs::WriteText( file, data.dump( true ) ))
        {
            auto *editor = GetCoreSystem( Editor );

            UWarning( "Could not write to archetype file.\nfile: %s",
                file.c_str( )
            );

            NotificationConfig error;

            error.type = NOTIFY_ERROR;
            error.header = "Save Error";
            error.message = "Unable to save archetype.";

            error.buttons =
            {
                { "Open Error Log", doOpenErrorLog }
            };

            editor->PostNotification( error );
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

    Json::object inspectComponent(ecs::Component *component)
    {
        meta::Variant instance { component, meta::variant_policy::WrapObject( ) };

        auto type = instance.GetType( );

        return Json::object {
            { "type", type.GetName( ) },
            { "value", type.SerializeJson( instance, editorGetterOverride, false ) },
            { "buttons", InspectComponentButtons( instance ) }
        };
    }
}

JSConstructor(EntityHandler)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid constructor arguments." );

    m_handle = arguments[ 0 ]->GetUIntValue( );
    m_scene = GetCoreSystem( Editor )->GetProject( )->GetScene( );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::isValid)
{
    return CefV8Value::CreateBool( getEntity( ) != nullptr );
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::isRemovalEnabled)
{
    auto entity = getEntity( );

    return CefV8Value::CreateBool( entity && entity->IsDeletionEnabled( ) );
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::isHierarchyChangeEnabled)
{
    auto entity = getEntity( );

    return CefV8Value::CreateBool( entity && entity->IsHierarchyChangeEnabled( ) );
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::isVisibleInEditor)
{
    auto entity = getEntity( );

    return CefV8Value::CreateBool( entity && entity->IsVisibleInEditor( ) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::remove)
{
    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    entity->Delete( );

    return CefV8Value::CreateBool( true );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::getName)
{
    auto entity = getEntity( );

    if (entity)
        return CefV8Value::CreateString( entity->GetName( ) );

    return CefV8Value::CreateBool( false );
}

///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::inspect)
{
    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto components = entity->GetComponents( );

    Json::array componentArray;

    for (auto *component : components)
        componentArray.emplace_back( ::inspectComponent( component ) );

    CefRefPtr<CefV8Value> output;

    JsonSerializer::Deserialize( componentArray, output );

    return output;
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::inspectComponent)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid constructor arguments.", nullptr );

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

    auto component = entity->GetComponent( id );

    if (!component)
        return CefV8Value::CreateBool( false );

    CefRefPtr<CefV8Value> output;

    JsonSerializer::Deserialize( ::inspectComponent( component ), output );

    return output;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

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

    ecs::ComponentTypeMask mask;

    mask.set( id, true );

    return CefV8Value::CreateBool( entity->HasComponent( mask ) );
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::addComponent)
{
    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto componentName = arguments[ 0 ]->GetStringValue( ).ToString( );

    auto componentType = meta::Type::GetFromName( componentName );

    if (!componentType.IsValid( ))
        return CefV8Value::CreateBool( false );

    auto componentID = componentType.GetStaticField( "ComponentID" );

    if (!componentID.IsValid( ))
        JSThrow( "Invalid component type.", nullptr );

    ecs::ComponentTypeMask componentTypeMask;

    componentTypeMask.set( componentID.GetValue( ).GetValue<ecs::ComponentTypeID>( ), true );

    // have this run in the main thread
    Application::Instance->ExecuteOnMainThread( [=] {
        if (!entity->HasComponent( componentTypeMask ))
        {
            auto instance = componentType.CreateDynamic( );

            entity->AddComponent( instance.GetValue<ecs::Component*>( ) );
        }
    } );

    return CefV8Value::CreateBool( true );
}

///////////////////////////////////////////////////////////////////////////////

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
    Application::Instance->ExecuteOnMainThread( [=] {
        entity->RemoveComponent( id );
    } );

    return CefV8Value::CreateBool( true );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::componentSet)
{
    if (arguments.size( ) != 2)
        JSThrow( "Invalid arguments.", nullptr );

    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto componentName = arguments[ 0 ]->GetStringValue( ).ToString( );

    auto componentType = meta::Type::GetFromName( componentName );

    if (!componentType.IsValid( ))
        JSThrow( "Unknown component type.", nullptr );

    auto &componentID = componentType.GetStaticField( "ComponentID" );

    if (!componentID.IsValid( ))
        JSThrow( "Invalid component type.", nullptr );

    auto *component = entity->GetComponent( 
        componentID.GetValue( ).GetValue<ecs::ComponentTypeID>( ) 
    );

    if (!component)
        return CefV8Value::CreateBool( false );

    auto value = JsonSerializer::Serialize( arguments[ 1 ] );

    meta::Variant instance { component, meta::variant_policy::WrapObject( ) };

    componentType.DeserializeJson( instance, value );

    return CefV8Value::CreateUndefined( );
}

JSMethod(EntityHandler::componentFieldUpdate)
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

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::componentFieldArrayUpdate)
{
    if (arguments.size( ) != 4)
        JSThrow( "Invalid arguments.", nullptr );

    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto componentName = arguments[ 0 ]->GetStringValue( ).ToString( );
    auto fieldName = arguments[ 1 ]->GetStringValue( ).ToString( );
    auto arrayIndex = static_cast<size_t>( arguments[ 2 ]->GetUIntValue( ) );

    auto componentType = meta::Type::GetFromName( componentName );

    if (!componentType.IsValid( ))
        JSThrow( "Unknown component type.", nullptr );

    auto &componentID = componentType.GetStaticField( "ComponentID" );

    if (!componentID.IsValid( ))
        JSThrow( "Invalid component type.", nullptr );

    auto *component = entity->GetComponent( 
        componentID.GetValue( ).GetValue<ecs::ComponentTypeID>( ) 
    );

    meta::Variant instance { component, meta::variant_policy::WrapObject( ) };

    auto field = componentType.GetField( fieldName );

    if (!field.IsValid( ))
        JSThrow( "Invalid field.", nullptr );

    auto fieldType = field.GetType( );

    if (!fieldType.IsArray( ))
        JSThrow( "Field is not an array type.", nullptr );

    auto fieldInstance = field.GetValue( instance );

    auto arrayWrapper = fieldInstance.GetArray( );
    
    auto value = JsonSerializer::Serialize( arguments[ 3 ] );
    auto valueToSet = fieldType.GetArrayType( ).DeserializeJson( value );

    arrayWrapper.SetValue( arrayIndex, valueToSet );

    return CefV8Value::CreateBool( true );
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::componentFieldArrayInsert)
{
    if (arguments.size( ) != 4)
        JSThrow( "Invalid arguments.", nullptr );

    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto componentName = arguments[ 0 ]->GetStringValue( ).ToString( );
    auto fieldName = arguments[ 1 ]->GetStringValue( ).ToString( );
    auto arrayIndex = static_cast<size_t>( arguments[ 2 ]->GetUIntValue( ) );

    auto componentType = meta::Type::GetFromName( componentName );

    if (!componentType.IsValid( ))
        JSThrow( "Unknown component type.", nullptr );

    auto &componentID = componentType.GetStaticField( "ComponentID" );

    if (!componentID.IsValid( ))
        JSThrow( "Invalid component type.", nullptr );

    auto *component = entity->GetComponent( 
        componentID.GetValue( ).GetValue<ecs::ComponentTypeID>( ) 
    );

    meta::Variant instance { component, meta::variant_policy::WrapObject( ) };

    auto field = componentType.GetField( fieldName );

    if (!field.IsValid( ))
        JSThrow( "Invalid field.", nullptr );

    auto fieldType = field.GetType( );

    if (!fieldType.IsArray( ))
        JSThrow( "Field is not an array type.", nullptr );

    auto fieldInstance = field.GetValue( instance );

    auto arrayWrapper = fieldInstance.GetArray( );
    
    auto value = JsonSerializer::Serialize( arguments[ 3 ] );
    auto valueToInsert = fieldType.GetArrayType( ).DeserializeJson( value );

    arrayWrapper.Insert( arrayIndex, valueToInsert );

    return CefV8Value::CreateBool( true );
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::componentFieldArrayRemove)
{
    if (arguments.size( ) != 3)
        JSThrow( "Invalid arguments.", nullptr );

    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto componentName = arguments[ 0 ]->GetStringValue( ).ToString( );
    auto fieldName = arguments[ 1 ]->GetStringValue( ).ToString( );
    auto arrayIndex = static_cast<size_t>( arguments[ 2 ]->GetUIntValue( ) );

    auto componentType = meta::Type::GetFromName( componentName );

    if (!componentType.IsValid( ))
        JSThrow( "Unknown component type.", nullptr );

    auto &componentID = componentType.GetStaticField( "ComponentID" );

    if (!componentID.IsValid( ))
        JSThrow( "Invalid component type.", nullptr );

    auto *component = entity->GetComponent( 
        componentID.GetValue( ).GetValue<ecs::ComponentTypeID>( ) 
    );

    meta::Variant instance { component, meta::variant_policy::WrapObject( ) };

    auto field = componentType.GetField( fieldName );

    if (!field.IsValid( ))
        JSThrow( "Invalid field.", nullptr );

    auto fieldType = field.GetType( );

    if (!fieldType.IsArray( ))
        JSThrow( "Field is not an array type.", nullptr );

    auto fieldInstance = field.GetValue( instance );

    auto arrayWrapper = fieldInstance.GetArray( );

    arrayWrapper.Remove( arrayIndex );

    return CefV8Value::CreateBool( true );
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::componentFieldArraySwap)
{
    if (arguments.size( ) != 4)
        JSThrow( "Invalid arguments.", nullptr );

    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto componentName = arguments[ 0 ]->GetStringValue( ).ToString( );
    auto fieldName = arguments[ 1 ]->GetStringValue( ).ToString( );
    auto arrayIndex1 = static_cast<size_t>( arguments[ 2 ]->GetUIntValue( ) );
    auto arrayIndex2 = static_cast<size_t>( arguments[ 3 ]->GetUIntValue( ) );

    auto componentType = meta::Type::GetFromName( componentName );

    if (!componentType.IsValid( ))
        JSThrow( "Unknown component type.", nullptr );

    auto &componentID = componentType.GetStaticField( "ComponentID" );

    if (!componentID.IsValid( ))
        JSThrow( "Invalid component type.", nullptr );

    auto *component = entity->GetComponent( 
        componentID.GetValue( ).GetValue<ecs::ComponentTypeID>( ) 
    );

    meta::Variant instance { component, meta::variant_policy::WrapObject( ) };

    auto field = componentType.GetField( fieldName );

    if (!field.IsValid( ))
        JSThrow( "Invalid field.", nullptr );

    auto fieldType = field.GetType( );

    if (!fieldType.IsArray( ))
        JSThrow( "Field is not an array type.", nullptr );

    auto fieldInstance = field.GetValue( instance );

    auto arrayWrapper = fieldInstance.GetArray( );

    auto value1 = arrayWrapper.GetValue( arrayIndex1 );
    auto value2 = arrayWrapper.GetValue( arrayIndex2 );

    arrayWrapper.SetValue( arrayIndex1, value2 );
    arrayWrapper.SetValue( arrayIndex2, value1 );

    return CefV8Value::CreateBool( true );
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::componentFieldArrayGetLength)
{
    if (arguments.size( ) != 2)
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

    meta::Variant instance { component, meta::variant_policy::WrapObject( ) };

    auto field = componentType.GetField( fieldName );

    if (!field.IsValid( ))
        JSThrow( "Invalid field.", nullptr );

    auto fieldType = field.GetType( );

    if (!fieldType.IsArray( ))
        JSThrow( "Field is not an array type.", nullptr );

    auto fieldInstance = field.GetValue( instance );

    auto arrayWrapper = fieldInstance.GetArray( );

    return CefV8Value::CreateUInt(
        static_cast<unsigned>( arrayWrapper.Size( ) )
    );
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::componentButtonInvoke)
{
    if (arguments.size( ) != 2)
        JSThrow( "Invalid arguments.", nullptr );

    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto componentName = arguments[ 0 ]->GetStringValue( ).ToString( );
    auto buttonName = arguments[ 1 ]->GetStringValue( ).ToString( );

    auto componentType = meta::Type::GetFromName( componentName );

    if (!componentType.IsValid( ))
        JSThrow( "Unknown component type.", nullptr );

    auto &componentID = componentType.GetStaticField( "ComponentID" );

    if (!componentID.IsValid( ))
        JSThrow( "Invalid component type.", nullptr );

    auto *component = entity->GetComponent( 
        componentID.GetValue( ).GetValue<ecs::ComponentTypeID>( ) 
    );

    meta::Variant instance { component, meta::variant_policy::WrapObject( ) };

    auto method = componentType.GetMethod( buttonName );

    if (!method.IsValid( ))
        JSThrow( "Invalid button name.", nullptr );

    UAssert( method.GetSignature( ).empty( ), 
        "Component buttons must have signature void(void).\n"
        "Component: %s, Button: %s",
        componentType.GetName( ).c_str( ),
        buttonName.c_str( )
    );

    method.Invoke( instance );

    return CefV8Value::CreateUndefined( );
}

///////////////////////////////////////////////////////////////////////////////

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
        auto *child = m_scene->GetWorld( )->GetEntity( children[ i ] );

        childrenArray->SetValue( static_cast<int>( i ), 
            CefV8Value::CreateUInt( child->GetUniqueID( ) ) 
        );
    }

    return childrenArray;
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::getParent)
{
    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto parent = entity->GetTransform( )->GetParent( );

    if (!parent)
        return CefV8Value::CreateNull( );

    return CefV8Value::CreateUInt( parent->GetOwner( )->GetUniqueID( ) );
}

///////////////////////////////////////////////////////////////////////////////

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
        auto currentParent = transform->GetParent( );

        if (currentParent)
            currentParent->RemoveChild( transform );
    }
    else
    {
        auto *targetEntity = 
            m_scene->GetWorld( )->GetEntityUnique( targetParent->GetUIntValue( ) );

        if (!targetEntity)
            return CefV8Value::CreateBool( false );

        targetEntity->GetTransform( )->AddChild( transform );
    }

    return CefV8Value::CreateBool( true );
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::getSiblingIndex)
{
    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto index = entity->GetTransform( )->GetSiblingIndex( );

    return CefV8Value::CreateUInt( index );
}

///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::clone)
{
    auto entity = getEntity( );

    if (!entity)
        return CefV8Value::CreateBool( false );

    auto clone = entity->Clone( );

    return CefV8Value::CreateUInt( clone->GetUniqueID( ) );
}

///////////////////////////////////////////////////////////////////////////////

ecs::Entity *EntityHandler::getEntity(void)
{
    return m_scene->GetWorld( )->GetEntityUnique( m_handle );
}