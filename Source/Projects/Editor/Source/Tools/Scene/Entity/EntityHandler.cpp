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

#include <Meta.h>
#include <ComponentConfig.h>

#include <FileSystem.h>
#include <FileDialog.h>
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
        const ecs::EntityHandle &entity, 
        int selectedFilter, 
        const fs::FileList &files
    )
    {
        if (files.empty( ))
            return;

        auto data = ecs::EntitySerializer( ).SerializeArchetype( entity );
        
        auto file = files[ 0 ].string( );

        if (!fs::WriteAllText( file, data.dump( true ) ))
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
        auto instance = ObjectVariant( component );

        auto type = instance.GetType( );

        // whether or not to force the serializtion hook
        auto forceHook = component->GetType( ).GetMeta( ).GetProperty<ForceSerializationHook>( ) != nullptr;

        return Json::object {
            { "type", type.GetName( ) },
            { "value", type.SerializeJson( instance, editorGetterOverride, forceHook ) },
            { "buttons", InspectComponentButtons( instance ) }
        };
    }
}

JSConstructor(EntityHandler)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid constructor arguments." );

    m_scene = &GetCoreSystem( Editor )->GetProject( )->GetScene( );
    m_world = m_scene->GetActiveWorld( );

    if (!m_world)
        JSThrow( "No active world." );

    auto id = arguments[ 0 ]->GetUIntValue( );
    
    m_handle = m_world->GetEntityManager( )->GetEntityByID( id );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::isValid)
{
    return CefV8Value::CreateBool( getHandle( ).IsValid( ) );
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::isRemovalEnabled)
{
    auto &handle = getHandle( );

    return CefV8Value::CreateBool( handle && handle->IsDeletionEnabled( ) );
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::isHierarchyChangeEnabled)
{
    auto &handle = getHandle( );

    return CefV8Value::CreateBool( handle && handle->IsHierarchyChangeEnabled( ) );
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::isVisibleInEditor)
{
    auto &handle = getHandle( );

    return CefV8Value::CreateBool( handle && handle->IsVisibleInEditor( ) );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::remove)
{
    auto &handle = getHandle( );

    if (!handle)
        return CefV8Value::CreateBool( false );

    handle->Delete( );

    return CefV8Value::CreateBool( true );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::getName)
{
    auto &handle = getHandle( );

    if (handle)
        return CefV8Value::CreateString( handle->GetName( ) );

    return CefV8Value::CreateBool( false );
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::setName)
{
    auto &handle = getHandle( );

    if (handle)
    {
        handle->SetName( arguments[ 0 ]->GetStringValue( ) );

        return CefV8Value::CreateBool( true );
    }

    return CefV8Value::CreateBool( false );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::inspect)
{
    auto &handle = getHandle( );

    if (!handle)
        return CefV8Value::CreateBool( false );

    auto components = handle->GetComponents( );

    // sort the components based on their instance ID
    sort( components.begin( ), components.end( ), ecs::EntityManager::CompareComponentsAscending );

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

    auto &handle = getHandle( );

    if (!handle)
        return CefV8Value::CreateBool( false );

    auto componentName = arguments[ 0 ]->GetStringValue( ).ToString( );

    auto componentType = meta::Type::GetFromName( componentName );

    if (!componentType.IsValid( ))
        return CefV8Value::CreateBool( false );

    auto &componentID = componentType.GetStaticField( "ComponentID" );

    if (!componentID.IsValid( ))
        return CefV8Value::CreateBool( false );

    auto id = componentID.GetValue( ).GetValue<ecs::ComponentTypeID>( );

    auto component = handle->GetComponent( id );

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
    auto &handle = getHandle( );

    if (!handle)
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

    return CefV8Value::CreateBool( handle->HasComponent( mask ) );
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::addComponent)
{
    auto &handle = getHandle( );

    if (!handle)
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
    Application::PostMainThread( [=] {
        if (!handle->HasComponent( componentTypeMask ))
        {
            auto instance = componentType.CreateDynamic( );

            handle->AddComponent( instance.GetValue<ecs::Component*>( ) );
        }
    } );

    return CefV8Value::CreateBool( true );
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::removeComponent)
{
    auto &handle = getHandle( );

    if (!handle)
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
    Application::PostMainThread( [=] {
        handle->RemoveComponent( id );
    } );

    return CefV8Value::CreateBool( true );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::componentSet)
{
    if (arguments.size( ) != 2)
        JSThrow( "Invalid arguments.", nullptr );

    auto &handle = getHandle( );

    if (!handle)
        return CefV8Value::CreateBool( false );

    auto componentName = arguments[ 0 ]->GetStringValue( ).ToString( );

    auto componentType = meta::Type::GetFromName( componentName );

    if (!componentType.IsValid( ))
        JSThrow( "Unknown component type.", nullptr );

    auto &componentID = componentType.GetStaticField( "ComponentID" );

    if (!componentID.IsValid( ))
        JSThrow( "Invalid component type.", nullptr );

    auto *component = handle->GetComponent(
        componentID.GetValue( ).GetValue<ecs::ComponentTypeID>( ) 
    );

    if (!component)
        return CefV8Value::CreateBool( false );

    auto value = JsonSerializer::Serialize( arguments[ 1 ] );

    auto instance = ObjectVariant( component );

    componentType.DeserializeJson( instance, value );

    return CefV8Value::CreateUndefined( );
}

JSMethod(EntityHandler::componentFieldUpdate)
{
    if (arguments.size( ) != 3)
        JSThrow( "Invalid arguments.", nullptr );

    auto &handle = getHandle( );

    if (!handle)
        return CefV8Value::CreateBool( false );

    auto componentName = arguments[ 0 ]->GetStringValue( ).ToString( );
    auto fieldName = arguments[ 1 ]->GetStringValue( ).ToString( );

    auto componentType = meta::Type::GetFromName( componentName );

    if (!componentType.IsValid( ))
        JSThrow( "Unknown component type.", nullptr );

    auto &componentID = componentType.GetStaticField( "ComponentID" );

    if (!componentID.IsValid( ))
        JSThrow( "Invalid component type.", nullptr );

    auto *component = handle->GetComponent(
        componentID.GetValue( ).GetValue<ecs::ComponentTypeID>( ) 
    );

    if (!component)
        JSThrow( "Component doesn't exist.", nullptr );

    auto value = JsonSerializer::Serialize( arguments[ 2 ] );

    auto instance = ObjectVariant( component );

    auto field = componentType.GetField( fieldName );

    if (!field.IsValid( ))
        JSThrow( "Invalid field.", nullptr );

    auto fieldType = field.GetType( );

    auto &fieldMeta = field.GetMeta( );

    auto *editorSetter = fieldMeta.GetProperty<EditorSetter>( );

    auto valueToSet = fieldType.DeserializeJson( value );

    Application::PostMainThread( [=](void) mutable
    {
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
    } );

    return CefV8Value::CreateUndefined( );
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::componentFieldArrayUpdate)
{
    if (arguments.size( ) != 4)
        JSThrow( "Invalid arguments.", nullptr );

    auto &handle = getHandle( );

    if (!handle)
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

    auto *component = handle->GetComponent(
        componentID.GetValue( ).GetValue<ecs::ComponentTypeID>( ) 
    );

    if (!component)
        JSThrow( "Component doesn't exist.", nullptr );

    auto instance = ObjectVariant( component );

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

    auto &handle = getHandle( );

    if (!handle)
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

    auto *component = handle->GetComponent(
        componentID.GetValue( ).GetValue<ecs::ComponentTypeID>( ) 
    );

    if (!component)
        JSThrow( "Component doesn't exist.", nullptr );

    auto instance = ObjectVariant( component );

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

    auto &handle = getHandle( );

    if (!handle)
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

    auto *component = handle->GetComponent(
        componentID.GetValue( ).GetValue<ecs::ComponentTypeID>( ) 
    );

    if (!component)
        JSThrow( "Component doesn't exist.", nullptr );

    auto instance = ObjectVariant( component );

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

    auto &handle = getHandle( );

    if (!handle)
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

    auto *component = handle->GetComponent(
        componentID.GetValue( ).GetValue<ecs::ComponentTypeID>( ) 
    );

    if (!component)
        JSThrow( "Component doesn't exist.", nullptr );

    auto instance = ObjectVariant( component );

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

    auto &handle = getHandle( );

    if (!handle)
        return CefV8Value::CreateBool( false );

    auto componentName = arguments[ 0 ]->GetStringValue( ).ToString( );
    auto fieldName = arguments[ 1 ]->GetStringValue( ).ToString( );

    auto componentType = meta::Type::GetFromName( componentName );

    if (!componentType.IsValid( ))
        JSThrow( "Unknown component type.", nullptr );

    auto &componentID = componentType.GetStaticField( "ComponentID" );

    if (!componentID.IsValid( ))
        JSThrow( "Invalid component type.", nullptr );

    auto *component = handle->GetComponent(
        componentID.GetValue( ).GetValue<ecs::ComponentTypeID>( ) 
    );

    if (!component)
        JSThrow( "Component doesn't exist.", nullptr );

    auto instance = ObjectVariant( component );

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

    auto &handle = getHandle( );

    if (!handle)
        return CefV8Value::CreateBool( false );

    auto componentName = arguments[ 0 ]->GetStringValue( ).ToString( );
    auto buttonName = arguments[ 1 ]->GetStringValue( ).ToString( );

    auto componentType = meta::Type::GetFromName( componentName );

    if (!componentType.IsValid( ))
        JSThrow( "Unknown component type.", nullptr );

    auto &componentID = componentType.GetStaticField( "ComponentID" );

    if (!componentID.IsValid( ))
        JSThrow( "Invalid component type.", nullptr );

    auto *component = handle->GetComponent(
        componentID.GetValue( ).GetValue<ecs::ComponentTypeID>( ) 
    );

    auto instance = ObjectVariant( component );

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
    auto &handle = getHandle( );

    if (!handle)
        return CefV8Value::CreateBool( false );

    auto &children = *handle->GetChildren( );

    auto childrenArray = CefV8Value::CreateArray( 
        static_cast<int>( children.size( ) ) 
    );

    for (size_t i = 0; i < children.size( ); ++i)
    {
        auto child = handle->GetWorld( )->GetEntity( children[ i ] );

        childrenArray->SetValue( static_cast<int>( i ), 
            CefV8Value::CreateUInt( child->GetID( ) ) 
        );
    }

    return childrenArray;
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::getParent)
{
    auto &handle = getHandle( );

    if (!handle)
        return CefV8Value::CreateBool( false );

    auto parent = handle->GetTransform( )->GetParent( );

    if (!parent)
        return CefV8Value::CreateNull( );

    return CefV8Value::CreateUInt( parent->GetOwner( )->GetID( ) );
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::setParent)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid arguments.", nullptr );

    auto &handle = getHandle( );

    if (!handle)
        return CefV8Value::CreateBool( false );

    auto *transform = handle->GetTransform( );

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
        auto targetEntity = handle->GetWorld( )->GetEntity( targetParent->GetUIntValue( ) );

        if (!targetEntity)
            return CefV8Value::CreateBool( false );

        targetEntity->GetTransform( )->AddChild( transform );
    }

    return CefV8Value::CreateBool( true );
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::getSiblingIndex)
{
    auto &handle = getHandle( );

    if (!handle)
        return CefV8Value::CreateBool( false );

    auto index = handle->GetTransform( )->GetSiblingIndex( );

    return CefV8Value::CreateUInt( index );
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::setSiblingIndex)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid arguments.", nullptr );

    auto &handle = getHandle( );

    if (!handle)
        return CefV8Value::CreateBool( false );

    handle->GetTransform( )->SetSiblingIndex( 
        arguments[ 0 ]->GetUIntValue( ) 
    );

    return CefV8Value::CreateBool( true );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::saveAsArchetype)
{
    auto &handle = getHandle( );

    if (!handle)
        return CefV8Value::CreateBool( false );

    auto *editor = GetCoreSystem( Editor );

     fs::FileDialog saveDialog;

    saveDialog.config.mode = fs::FDM_SAVE;
    saveDialog.config.initialPath = "";
    saveDialog.config.windowTitle = "Save Archetype";
    saveDialog.config.parentWindow = editor->GetMainWindow( ).GetWindow( );
    saveDialog.config.filters = {
        { "Archetype Files", { "*.uatype" } }
    };

    auto result = saveDialog.Open( );

    doSaveArchetype( handle, result.selectedFilterIndex, result.selectedFiles );

    return CefV8Value::CreateBool( true );
}

///////////////////////////////////////////////////////////////////////////////

JSMethod(EntityHandler::clone)
{
    auto &handle = getHandle( );

    if (!handle)
        return CefV8Value::CreateBool( false );

    auto clone = handle->Clone( );

    return CefV8Value::CreateUInt( clone->GetID( ) );
}

const ecs::EntityHandle &EntityHandler::getHandle(void)
{
    auto *activeWorld = m_scene->GetActiveWorld( );

    // make sure the world is still valid
    if (activeWorld != m_world)
        return ecs::EntityHandle::Invalid( );

    return m_handle;
}