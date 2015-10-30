#include "Precompiled.h"

#include "SceneUtils.h"

#include "Editor.h"
#include "Project.h"

#include <UIFileDialogCallback.h>

using namespace ursine;

JSFunction(SceneGetActiveEntities)
{
    auto &scene = GetCoreSystem( Editor )->GetProject( )->GetScene( );

    auto &active = scene.GetWorld( ).GetActiveEntities( );

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

#include <iostream>

Meta(Enable, ExposeJavaScript)
JSFunction(SceneLoad)
{
    auto *editor = GetCoreSystem( Editor );

    CefRefPtr<UIFileDialogCallback> callback = new UIFileDialogCallback( 
        [](int, std::vector<fs::path> paths) 
        {
            // TODO:
        } 
    );

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

    CefRefPtr<UIFileDialogCallback> callback = new UIFileDialogCallback( 
        [](int, std::vector<fs::path> paths) 
        {
            // TODO:
        } 
    );

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