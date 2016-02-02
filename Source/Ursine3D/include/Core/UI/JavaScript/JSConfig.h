/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** JavaScriptConfig.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <cef_v8.h>

#define JSThrow(message, ...) { exception = message; return __VA_ARGS__; }\

#define JSHandlerArgs                \
    CefRefPtr<CefV8Value> context,   \
    const CefV8ValueList &arguments, \
    CefString &exception             \

namespace ursine
{
    extern template
    Json JsonSerializer::Serialize(const CefRefPtr<CefV8Value> &instance);

    extern template
    void JsonSerializer::Deserialize(const Json &data, CefRefPtr<CefV8Value> &out);
}