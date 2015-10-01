#pragma once

#include <NativeJavaScriptFunction.h>

Meta(Enable, ExposeJavaScript)
JavaScriptFunction(DebugEditorUI)
{
    return CefV8Value::CreateInt( 69 );
}