/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** LauncherUtils.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "LauncherUtils.h"
#include "Editor.h"

JSFunction(GetEditorVersion)
{
    return CefV8Value::CreateString( EDITOR_VERSION );
}

JSFunction(LauncherProjectCreate)
{
    if (arguments.size( ) != 2)
        JSThrow( "Invalid arguments.", nullptr );

    auto projectName = arguments[ 0 ]->GetStringValue( );
    auto projectDirectory = arguments[ 1 ]->GetStringValue( );

    auto *editor = GetCoreSystem( Editor );

    editor->CreateNewProject( projectName, projectDirectory );

	return CefV8Value::CreateUndefined( );
}

JSFunction(LauncherProjectOpen)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid arguments.", nullptr );

    auto projectFile = arguments[ 0 ]->GetStringValue( );

    auto *editor = GetCoreSystem( Editor );

    editor->LoadProject( projectFile );

	return CefV8Value::CreateUndefined( );
}