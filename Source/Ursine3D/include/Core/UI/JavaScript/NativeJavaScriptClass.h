#pragma once

#include <include/cef_v8.h>

#define JavaScriptMethod(name)                                            \
    CefRefPtr<CefV8Value> name(CefRefPtr<CefV8Value> context,             \
                                         const CefV8ValueList &arguments, \
                                                    CefString &exception) \

namespace ursine
{
    class NativeJavaScriptClass
    {

    } Meta(Enable);
}