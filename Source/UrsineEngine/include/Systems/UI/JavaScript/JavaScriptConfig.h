/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

#include <functional>

#define JSMethod(name)                                                    \
    CefRefPtr<CefV8Value> name(CefRefPtr<CefV8Value> context,             \
                                         const CefV8ValueList &arguments, \
                                                    CefString &exception) \

#define BindJSMethod(name) std::bind(name, this, _1, _2, _3)

using namespace std::placeholders;

namespace ursine
{
    namespace ui
    {
        // this context, args, exception [out]
        typedef std::function<CefRefPtr<CefV8Value>(
            CefRefPtr<CefV8Value>, 
            const CefV8ValueList &,
            CefString &)> JavaScriptMethod;
    }

    extern template
    Json JsonSerializer::Serialize(const CefRefPtr<CefV8Value> &instance);

    extern template
    void JsonSerializer::Deserialize(const Json &data, CefRefPtr<CefV8Value> &out);
}