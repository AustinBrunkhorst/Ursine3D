#pragma once

namespace Ursine
{
    class JavaScriptAccessor : public CefV8Accessor
    {
    public:
        typedef std::unordered_map<std::string, CefRefPtr<CefV8Value>> Object;

        JavaScriptAccessor(const Object &object);

    private:
        Object _object;

        bool Get(const CefString &name,
            const CefRefPtr<CefV8Value> object,
            CefRefPtr<CefV8Value> &retval,
            CefString &exception) override;

        bool Set(const CefString &name,
            const CefRefPtr<CefV8Value> object,
            const CefRefPtr<CefV8Value> value,
            CefString &exception) override;

        IMPLEMENT_REFCOUNTING(JavaScriptAccessor);
    };
}
