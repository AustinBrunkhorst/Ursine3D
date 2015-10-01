#pragma once

#include <cef_v8.h>

#define JavaScriptFunction(name)                              \
    CefRefPtr<CefV8Value> name(CefRefPtr<CefV8Value> context, \
        const CefV8ValueList &arguments,                      \
        CefString &exception)                                 \

/** @brief Exposes a global or static function to JavaScript.
*/
struct ExposeJavaScript : ursine::meta::MetaProperty
{
    META_OBJECT
};