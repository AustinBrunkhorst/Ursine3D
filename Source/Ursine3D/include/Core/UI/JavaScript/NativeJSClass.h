/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** NativeJSClass.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Meta.h"

#include "JSConfig.h"

#define JSConstructor(name)                                      \
    Meta(DisableNonDynamic, WrapObject)                          \
    name::name(JSHandlerArgs, CefRefPtr<CefV8Value> thisContext) \

#define JSMethod(name)                        \
    CefRefPtr<CefV8Value> name(JSHandlerArgs) \

#define JAVASCRIPT_CLASS META_OBJECT

namespace ursine
{
    class NativeJSClass : public meta::Object
    {
    protected:
        NativeJSClass(void);

        void messageBrowser(const std::string &channel, const std::string &message, const Json &data) const;

    private:
        CefRefPtr<CefBrowser> m_browser;
    } Meta(Enable, WhiteListMethods);
}