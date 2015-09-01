/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** JavaScriptExtension.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


#include "UrsinePrecompiled.h"

#include "JavaScriptExtension.h"

namespace ursine
{
    namespace UI
    {
        bool JavaScriptExtension::Execute(const CefString &name,
            CefRefPtr<CefV8Value> object,
            const CefV8ValueList &arguments,
            CefRefPtr<CefV8Value> &retval,
            CefString &exception)
        {
            auto method = _binds.find(name);

            if (method == _binds.end())
                return false;

            retval = method->second(object, arguments, exception);

            return true;
        }

        void JavaScriptExtension::bind(const std::string &name,
            JavaScriptMethod method)
        {
            _binds[name] = method;
        }
    }
}