#include "Precompiled.h"

#include "SceneUtils.h"

#include "Editor.h"
#include "Project.h"

#include <WorldSerializer.h>
#include <UIFileDialogCallback.h>

#include <SDL_messagebox.h>

using namespace ursine;

namespace
{
    typedef std::vector<fs::path> FileList;

    void doLoadScene(int selectedFilter, const FileList &files);
    void doSaveScene(int selectedFilter, const FileList &files);
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

Meta(Enable, ExposeJavaScript)
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

Meta(Enable, ExposeJavaScript)
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

namespace
{
    void doLoadScene(int selectedFilter, const FileList &files)
    {
        if (files.empty( ))
            return;

        auto *editor = GetCoreSystem( Editor );

        ecs::WorldSerializer serializer;

        ecs::World::Handle world;

        if (!serializer.Deserialize( files[ 0 ].string( ), world ))
        {
            URSINE_TODO( "Use UI error popup" );
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "Load Error",
                "Unable to load world.",
                editor->GetMainWindow( )->GetInternalHandle( )
            );

            return;
        }

        editor->GetProject( )->SetWorld( world );
    }

    void doSaveScene(int selectedFilter, const FileList &files)
    {
        if (files.empty( ))
            return;

        auto *editor = GetCoreSystem( Editor );
        auto world = editor->GetProject( )->GetScene( )->GetWorld( );

        ecs::WorldSerializer serializer;

        auto serialized = serializer.Serialize( world );

        auto path = files[ 0 ];

        if (!fs::WriteText( path.string( ), serialized.dump( ) ))
        {
            URSINE_TODO( "Use UI error popup" );
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "Save Error",
                "Unable to save world.",
                editor->GetMainWindow( )->GetInternalHandle( )
            );
        }
    }
}