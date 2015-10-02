#pragma once

#include "JSConfig.h"

#define JSFunction(name)                      \
    CefRefPtr<CefV8Value> name(JSHandlerArgs) \

/** @brief Exposes a global or static function to JavaScript.
*/
struct ExposeJavaScript : ursine::meta::MetaProperty
{
    META_OBJECT
};