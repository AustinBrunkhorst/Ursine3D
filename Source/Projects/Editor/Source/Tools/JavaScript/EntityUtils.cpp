/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityUtils.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "EntityUtils.h"
#include "SelectedComponent.h"

#include <Editor.h>

#include <World.h>

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

    void doLoadArchetype(
        ecs::World *world,
        int selectedFilter,
        const fs::FileList &files
        )
    {
        if (files.empty( ))
            return;

        auto &file = files.front( );
        auto filename = file.stem( ).string( );

        try
        {
            std::string jsonData;

            if (!fs::LoadText( file.string( ), jsonData ))
            {
                throw ecs::SerializationException( "Unable to load file." );
            }
       
            std::string jsonError;

            auto data = Json::parse( jsonData, jsonError );

            if (!jsonError.empty( ))
            {
                throw ecs::SerializationException( jsonError );
            }

            auto entity = ecs::EntitySerializer( ).DeserializeArchetype( 
                world, 
                data 
            );

            entity->SetName( filename + " Archetype" );
        }
        catch (ecs::SerializationException &e)
        {
            UWarning(
                "Unable to load archetype.\nerror: %s\nfile: %s",
                e.GetError( ).c_str( ),
                file.string( ).c_str( )
            );

            auto *editor = GetCoreSystem( Editor );

            NotificationConfig error;

            error.type = NOTIFY_ERROR;
            error.header = "Load Error";
            error.message = "Unable to load archetype.";

            error.buttons =
            {
                { "Open Error Log", doOpenErrorLog }
            };

            editor->PostNotification( error );
        }
    }
}

JSFunction(CreateEntity)
{
    auto world = GetCoreSystem( Editor )->GetProject( )->GetScene( )->GetWorld( );

    auto *entity = world->CreateEntity( );

    return CefV8Value::CreateUInt( entity->GetUniqueID( ) );
}

JSFunction(CreateEntityFromArchetype)
{
    auto world = GetCoreSystem( Editor )->GetProject( )->GetScene( )->GetWorld( );

    CefRefPtr<UIFileDialogCallback> callback = 
        new UIFileDialogCallback( std::bind( &doLoadArchetype, world, _1, _2 ) );

    std::vector<CefString> filters {
        "Archetype Files|.uatype"
    };

    auto *editor = GetCoreSystem( Editor );

    editor->GetMainUI( )->GetBrowser( )->GetHost( )->RunFileDialog(
        FILE_DIALOG_OPEN,
        "Create Entity From Archetype",
        "",
        filters,
        0,
        callback
    );

    return CefV8Value::CreateBool( true );
}