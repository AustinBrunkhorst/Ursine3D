#include "UrsinePrecompiled.h"

#include "JavaScriptAccessor.h"

namespace ursine
{
    JavaScriptAccessor::JavaScriptAccessor(const Object &object) 
        : m_object(object) {}

    bool JavaScriptAccessor::Get(const CefString &name, 
        const CefRefPtr<CefV8Value> object, 
        CefRefPtr<CefV8Value> &retval, 
        CefString &exception)
    {
        printf("accessor: %s\n", name.ToString().c_str());

        auto search = m_object.find(name);

        if (search == m_object.end())
            exception = "Key does not exist.";
        else
            retval = search->second;

        return true;
    }

    bool JavaScriptAccessor::Set(const CefString &name, 
        const CefRefPtr<CefV8Value> object, 
        const CefRefPtr<CefV8Value> value, 
        CefString &exception)
    {
        m_object[name] = value;

        return true;
    }
}
