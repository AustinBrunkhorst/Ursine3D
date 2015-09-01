/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** JavaScriptExtension.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "JavaScriptConfig.h"

#define JS_SRC(...) #__VA_ARGS__

namespace ursine
{
    namespace UI
    {
        class JavaScriptExtension : public CefV8Handler
        {
            std::unordered_map<std::string, JavaScriptMethod> _binds;

            bool Execute(const CefString &name,
                CefRefPtr<CefV8Value> object,
                const CefV8ValueList &arguments,
                CefRefPtr<CefV8Value> &retval,
                CefString &exception) override;

        protected:
            // binds a function handler for this extension
            void bind(const std::string &name, JavaScriptMethod method);

        public:
            virtual ~JavaScriptExtension(void) { }

            // Registers this extension in v8. This must be called in
            // "OnWebKitInitialized"
            virtual void Register(void) = 0;

            IMPLEMENT_REFCOUNTING(JavaScriptExtension);
        };
    }
}