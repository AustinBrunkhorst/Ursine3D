#include "Precompiled.h"

#include "DebugUtils.h"

#include <UIManager.h>

JavaScriptFunction(DebugEditorUI)
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