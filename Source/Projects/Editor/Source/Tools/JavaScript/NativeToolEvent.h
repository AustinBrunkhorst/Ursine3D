#pragma once

#include "EventArgs.h"

#include <cef_v8.h>

struct NativeToolEvent : ursine::EventArgs
{
    std::string name;
    CefRefPtr<CefV8Value> data;

    NativeToolEvent(const std::string &name, CefRefPtr<CefV8Value> data)
        : name( name )
        , data( data ) { }
};

namespace native_tool
{
    namespace event
    {
        // Window container moused over
        const auto MouseOver = "mouseover";

        // Window container moused out
        const auto MouseOut = "mouseout";

        // Window container focused
        const auto Focus = "focus";

        // Window container focus lost
        const auto Blur = "blur";

        // Window container viewport changed
        const auto ViewportInvalidated = "viewportInvalidated";
    }
}