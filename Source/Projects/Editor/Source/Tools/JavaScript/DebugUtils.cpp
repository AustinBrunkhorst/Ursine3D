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

JSFunction(DebugEditorUI)
{
    std::string debugURL( "http://localhost:" );

    debugURL += std::to_string( ursine::UIManager::REMOTE_DEBUGGING_PORT );

#ifdef PLATFORM_WINDOWS

    ShellExecute(
        nullptr,
        "open",
        debugURL.c_str( ),
        nullptr,
        nullptr,
        SW_SHOWNORMAL
    );

#endif

    return CefV8Value::CreateBool( true );
}