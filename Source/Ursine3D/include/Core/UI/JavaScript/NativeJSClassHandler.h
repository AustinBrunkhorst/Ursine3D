/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** NativeJSClassHandler.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Meta.h"

#include <cef_v8.h>

namespace ursine
{
    class NativeJSClassHandler : public CefV8Handler
    {
    private:
        friend class RenderProcessHandler;

        struct InstanceWrapper : CefBase
        {
            meta::Variant instance;

            InstanceWrapper(
                const meta::Constructor &constructor,
                CefRefPtr<CefV8Value> context,
                const CefV8ValueList &arguments,
                CefString &exception,
                CefRefPtr<CefV8Value> thisContext
            );

            ~InstanceWrapper(void);

            IMPLEMENT_REFCOUNTING( InstanceWrapper );
        };

        struct PrototypeHandler : CefV8Handler
        {
            std::unordered_map<
                std::string,
                std::pair<CefRefPtr<CefV8Value>, meta::Method>
            > methods;

            bool Execute(
                const CefString &name,
                CefRefPtr<CefV8Value> context,
                const CefV8ValueList &arguments,
                CefRefPtr<CefV8Value> &retval,
                CefString &exception
            ) override;

            IMPLEMENT_REFCOUNTING( PrototypeHandler );
        };

        meta::Type m_classType;

        const meta::Constructor &m_constructor;

        CefRefPtr<PrototypeHandler> m_prototypeHandler;

        std::vector<CefRefPtr<InstanceWrapper>> m_instances;

        NativeJSClassHandler(meta::Type classType);

        void Bind(CefRefPtr<CefV8Value> object);
        void UnBind(CefRefPtr<CefV8Value> object);

        bool Execute(
            const CefString &name,
            CefRefPtr<CefV8Value> object,
            const CefV8ValueList &arguments,
            CefRefPtr<CefV8Value> &retval,
            CefString &exception
        ) override;

        IMPLEMENT_REFCOUNTING( NativeJSClassHandler );
    };
}