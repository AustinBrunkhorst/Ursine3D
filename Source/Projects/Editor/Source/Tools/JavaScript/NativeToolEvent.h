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