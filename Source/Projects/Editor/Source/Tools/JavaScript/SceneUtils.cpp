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
#include <WorldSerializer.h>
#include <WorldConfigComponent.h>
#include <SystemManager.h>
#include <Timer.h>

#include <FileSystem.h>
#include <UIFileDialogCallback.h>

using namespace ursine;

namespace
{
    void doLoadScene(int selectedFilter, const fs::FileList &files);
    void doSaveScene(int selectedFilter, const fs::FileList &files);

    void doOpenErrorLog(Notification &notification);
}

JSFunction(SceneGetRootEntities)
{
    auto scene = GetCoreSystem( Editor )->GetProject( )->GetScene( );

    auto root = scene->GetWorld( )->GetRootEntities( );

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
    auto scene = GetCoreSystem( Editor )->GetProject( )->GetScene( );

    auto &active = scene->GetWorld( )->GetActiveEntities( );

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

JSFunction(SceneLoad)
{
    auto *editor = GetCoreSystem( Editor );

    CefRefPtr<UIFileDialogCallback> callback = 
        new UIFileDialogCallback( doLoadScene );

    std::vector<CefString> filters {
        "World Files|.uworld"
    };

    editor->GetMainUI( )->GetBrowser( )->GetHost( )->RunFileDialog(
        FILE_DIALOG_OPEN,
        "Load World",
        "",
        filters,
        0,
        callback
    );

    return CefV8Value::CreateUndefined( );
}

JSFunction(SceneSave)
{
    auto *editor = GetCoreSystem( Editor );

    CefRefPtr<UIFileDialogCallback> callback = 
        new UIFileDialogCallback( doSaveScene );

    std::vector<CefString> filters {
        "World Files|.uworld"
    };

    editor->GetMainUI( )->GetBrowser( )->GetHost( )->RunFileDialog(
        static_cast<CefBrowserHost::FileDialogMode>( FILE_DIALOG_SAVE | FILE_DIALOG_OVERWRITEPROMPT_FLAG ),
        "Save World",
        "",
        filters,
        0,
        callback
    );

    return CefV8Value::CreateUndefined( );
}

JSFunction(ScenePlayStart)
{
    Application::Instance->ExecuteOnMainThread( [] {
        auto *editor = GetCoreSystem( Editor );

        editor->GetProject( )->SetPlayState( PS_PLAYING );
    } );

    return CefV8Value::CreateUndefined( );
}

JSFunction(SceneSetPlayState)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid arguments.", nullptr );

    auto playing = arguments[ 0 ]->GetBoolValue( );

    Application::Instance->ExecuteOnMainThread( [=] {
		auto *editor = GetCoreSystem( Editor );

        editor->GetProject( )->SetPlayState( playing ? PS_PLAYING : PS_PAUSED );
    } );

    return CefV8Value::CreateUndefined( );
}

JSFunction(SceneStep)
{
    Application::Instance->ExecuteOnMainThread( [=] {
		auto *editor = GetCoreSystem( Editor );

		editor->GetProject( )->GetScene( )->Step( );
    } );

    return CefV8Value::CreateUndefined( );
}

JSFunction(ScenePlayStop)
{
    Application::Instance->ExecuteOnMainThread( [] {
        auto *editor = GetCoreSystem( Editor );

        editor->GetProject( )->SetPlayState( PS_EDITOR );
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
    void doLoadScene(int selectedFilter, const fs::FileList &files)
    {
        if (files.empty( ))
            return;

        auto *editor = GetCoreSystem( Editor );

        auto file = files[ 0 ].string( );

        try
        {
			Application::Instance->ExecuteOnMainThread( [=] {
				ecs::WorldSerializer serializer;
				auto world = serializer.Deserialize( file );

                URSINE_TODO( "this is hacky and weirdly placed" );
                world->GetSettings( )->GetComponent<ecs::WorldConfig>( )->SetInEditorMode( true );

				editor->GetProject( )->SetWorld( world );
			} );
        }
        catch (const ecs::SerializationException &e)
        {
            UWarning( "World deserialization failure.\nfile: %s\n\n%s",
                file.c_str( ),
                e.GetError( ).c_str( ) 
            );

            NotificationConfig error;

            error.type = NOTIFY_ERROR;
            error.header = "Load Error";
            error.message = "Unable to load world.";
            
            error.buttons = 
            {
                { "Open Error Log", doOpenErrorLog }
            };

            editor->PostNotification( error );
        }
    }

    void doSaveScene(int selectedFilter, const fs::FileList &files)
    {
        if (files.empty( ))
            return;

        auto *editor = GetCoreSystem( Editor );
        auto world = editor->GetProject( )->GetScene( )->GetWorld( );

        ecs::WorldSerializer serializer;

        auto serialized = serializer.Serialize( world );

        auto path = files[ 0 ];

        if (!fs::WriteText( path.string( ), serialized.dump( true ) ))
        {
            UWarning( "Could not write to world file.\nfile: %s",
                path.string( ).c_str( )
            );

            NotificationConfig error;

            error.type = NOTIFY_ERROR;
            error.header = "Save Error";
            error.message = "Unable to save world.";

            error.buttons =
            {
                { "Open Error Log", doOpenErrorLog }
            };

            editor->PostNotification( error );
        }
    }

    void doOpenErrorLog(Notification &notification)
    {
        utils::OpenPath( URSINE_ERROR_LOG_FILE );
    }
}