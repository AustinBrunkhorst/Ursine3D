#pragma once

#include "Meta.h"

#include <cef_v8.h>

namespace ursine
{
    class NativeJSClassHandler : public CefV8Handler
    {
    private:
        friend class RenderProcessHandler;

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

        struct InstanceWrapper : CefBase
        {
            meta::Variant instance;

            InstanceWrapper(
                const meta::Constructor &constructor,
                CefRefPtr<CefV8Value> context,
                const CefV8ValueList &arguments,
                CefString &exception
            );

            IMPLEMENT_REFCOUNTING( InstanceWrapper );
        };

        meta::Type m_classType;

        meta::Constructor m_constructor;

        CefRefPtr<PrototypeHandler> m_prototypeHandler;

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