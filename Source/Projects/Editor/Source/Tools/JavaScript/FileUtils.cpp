/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FileUtils.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "FileUtils.h"
#include "Editor.h"

#include <FileSystem.h>
#include <UIFileDialogCallback.h>

using namespace ursine;
using namespace std::placeholders;

namespace
{
    void doBrowseFileCallback(int callbackID, int selectedFilter, const fs::PathList &files)
    {
        if (!files.empty( ))
        {
            auto *editor = GetCoreSystem( Editor );

            editor->GetMainWindow( ).GetUI( )->Message( UI_CMD_BROADCAST, "FileBrowser", "fileSelected", 
                Json::object {
                    { "callbackID", callbackID },
                    { "file", files[ 0 ].string( ) }
                } 
            );
        }
    }

    void runDialog(CefBrowserHost::FileDialogMode mode, const CefV8ValueList &arguments)
    {
        auto title = arguments[ 0 ]->GetStringValue( );
        auto filtersLength = arguments[ 1 ]->GetArrayLength( );
        auto callbackID = arguments[ 2 ]->GetIntValue( );
    
        CefRefPtr<UIFileDialogCallback> callback = 
            new UIFileDialogCallback( std::bind( &doBrowseFileCallback, callbackID, _1, _2 ) );

        std::vector<CefString> filters;

        for (auto i = 0; i < filtersLength; ++i)
            filters.emplace_back( arguments[ 1 ]->GetValue( i )->GetStringValue( ) );

        RunFileDialog(
            mode, 
            title, 
            "", 
            filters, 
            std::bind( &doBrowseFileCallback, callbackID, _1, _2 ) 
        );
    }
}

void RunFileDialog(
    CefBrowserHost::FileDialogMode mode,
    const std::string &title,
    const fs::path &defaultPath,
    const std::vector<CefString> &filters,
    UIFileDialogCallback::Callback callback
)
{
    auto *editor = GetCoreSystem( Editor );

    CefRefPtr<UIFileDialogCallback> callbackHandler = 
        new UIFileDialogCallback( callback );

    editor->GetMainWindow( ).GetUI( )
        ->GetBrowser( )
        ->GetHost( )
        ->RunFileDialog(
            mode,
            title,
            defaultPath.string( ),
            filters,
            0,
            callbackHandler
        );
}

JSFunction(BrowseFile)
{
    if (arguments.size( ) != 3)
        JSThrow( "Invalid arguments.", nullptr );

    runDialog( FILE_DIALOG_OPEN, arguments );

	return CefV8Value::CreateUndefined( );
}

JSFunction(BrowseFolder)
{
    if (arguments.size( ) != 3)
        JSThrow( "Invalid arguments.", nullptr );

    runDialog( FILE_DIALOG_OPEN_FOLDER, arguments );

	return CefV8Value::CreateUndefined( );
}
