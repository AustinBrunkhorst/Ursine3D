#pragma once

#include "Meta.h"
#include <include/cef_v8.h>

#define JavascriptConstructor       \
    CefRefPtr<CefV8Value> context,  \
    const CefV8ValueList &arguments \

#define JavaScriptMethod(name)                                   \
    CefRefPtr<CefV8Value> name(CefRefPtr<CefV8Value> context,    \
                                const CefV8ValueList &arguments, \
                                CefString &exception)            \

namespace ursine
{
    class NativeJavaScriptClass
    {

    } Meta(Enable);
}