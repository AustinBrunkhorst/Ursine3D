#pragma once

#include <cef_v8.h>

namespace ursine
{
    class NativeJSFunctionHandler : public CefV8Handler
    {
    public:
        void Bind(CefRefPtr<CefV8Value> context);

    private:
        std::unordered_map<std::string, meta::Function> m_functions;

        bool Execute(const CefString &name,
            CefRefPtr<CefV8Value> object,
            const CefV8ValueList &arguments,
            CefRefPtr<CefV8Value> &retval,
            CefString &exception) override;

        IMPLEMENT_REFCOUNTING( NativeJSFunctionHandler );
    };
}