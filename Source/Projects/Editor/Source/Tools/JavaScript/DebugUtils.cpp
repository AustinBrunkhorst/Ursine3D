/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DebugUtils.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "DebugUtils.h"

#include <UIManager.h>

#include <FileSystem.h>

JSFunction(DebugEditorUI)
{
    std::string debugURL( "http://localhost:" );

    debugURL += std::to_string( ursine::UIManager::REMOTE_DEBUGGING_PORT );

    ursine::utils::OpenPath( debugURL );

    return CefV8Value::CreateBool( true );
}

JSFunction(ProcessOpen)
{
	if (arguments.size( ) != 2)
		JSThrow( "Invalid arguments.", nullptr );

	auto path = arguments[ 0 ]->GetStringValue( ).ToString( );
	auto relative = arguments[ 1 ]->GetBoolValue( );

	std::string outputPath;

	if (relative)
	{
		outputPath = (ursine::fs::current_path( ) / path).string( );
	}
	else
	{
		outputPath = path;
	}

    ursine::utils::OpenPath( outputPath );

    return CefV8Value::CreateBool( true );
}