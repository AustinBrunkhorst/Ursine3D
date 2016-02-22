/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SceneUtils.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "SceneUtils.h"

#include "Editor.h"
#include "Project.h"

#include <SystemManager.h>
#include <WorldConfigComponent.h>
#include <SystemManager.h>
#include <Timer.h>

using namespace ursine;

namespace
{
    Scene &getScene(void);
    ecs::World *getActiveWorld(void);

    void doOpenErrorLog(Notification &notification);
}

JSFunction(SceneSaveWorld)
{
    return CefV8Value::CreateBool( true );
}

JSFunction(SceneSaveWorldAs)
{
    return CefV8Value::CreateBool( true );
}

JSFunction(SceneSetActiveWorld)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid arguments.", nullptr );

    auto guid = GUIDStringGenerator( )( arguments[ 0 ]->GetStringValue( ).ToString( ) );

    auto &scene = getScene( );

    auto reference = scene.GetResourceManager( ).CreateReference( guid );

    scene.SetActiveWorld( reference );

    return CefV8Value::CreateBool( true );
}

JSFunction(SceneGetRootEntities)
{
    auto *world = getActiveWorld( );
    
    if (!world)
        return CefV8Value::CreateArray( 0 );

    auto root = world->GetRootEntities( );

    auto ids = CefV8Value::CreateArray( static_cast<int>( root.size( ) ) );

    for (size_t i = 0; i < root.size( ); ++i)
    {
        ids->SetValue( 
            static_cast<int>( i ), 
            CefV8Value::CreateUInt( root[ i ]->GetUniqueID( ) )
        );
    }

    return ids;
}

JSFunction(SceneGetActiveEntities)
{
    auto *world = getActiveWorld( );
    
    if (!world)
        return CefV8Value::CreateArray( 0 );

    auto &active = world->GetActiveEntities( );

    auto ids = CefV8Value::CreateArray( static_cast<int>( active.size( ) ) );

    for (size_t i = 0; i < active.size( ); ++i)
    {
        ids->SetValue( 
            static_cast<int>( i ), 
            CefV8Value::CreateUInt( active[ i ]->GetUniqueID( ) )
        );
    }

    return ids;
}

JSFunction(ScenePlayStart)
{
    Timer::Create( 0 ).Completed( [] {
        auto &scene = getScene( );

        scene.SetPlayState( PS_PLAYING );
    } );

    return CefV8Value::CreateUndefined( );
}

JSFunction(SceneSetPlayState)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid arguments.", nullptr );

    auto playing = arguments[ 0 ]->GetBoolValue( );

    Timer::Create( 0 ).Completed( [=] {
        auto &scene = getScene( );

        scene.SetPlayState( playing ? PS_PLAYING : PS_PAUSED );
    } );

    return CefV8Value::CreateUndefined( );
}

JSFunction(SceneStep)
{
    Timer::Create( 0 ).Completed( [=] {
        auto &scene = getScene( );

        scene.Step( );
    } );

    return CefV8Value::CreateUndefined( );
}

JSFunction(ScenePlayStop)
{
    Timer::Create( 0 ).Completed( [] {
        auto &scene = getScene( );

        scene.SetPlayState( PS_EDITOR );
    } );

    return CefV8Value::CreateUndefined( );
}

JSFunction(SceneGetEntitySystems)
{
    Json::array systems;

    for (auto &type : ecs::SystemManager::GetExposedTypes( ))
    {
        auto &meta = type.GetMeta( );

        // exclude auto added systems
        if (!meta.GetProperty<AutoAddEntitySystem>( ))
            systems.emplace_back( type.GetName( ) );
    }

    CefRefPtr<CefV8Value> items;

    JsonSerializer::Deserialize( systems, items );

    return items;
}

namespace
{
    Scene &getScene(void)
    {
        auto *editor = GetCoreSystem( Editor );

        return editor->GetProject( )->GetScene( );
    }

    ecs::World *getActiveWorld(void)
    {
        return getScene( ).GetActiveWorld( );
    }
}