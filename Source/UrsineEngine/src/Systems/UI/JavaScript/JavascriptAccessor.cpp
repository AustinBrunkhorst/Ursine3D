#include "UrsinePrecompiled.h"

#include "JavaScriptAccessor.h"

namespace Ursine
{
    JavaScriptAccessor::JavaScriptAccessor(const Object &object) 
        : _object(object) {}

    bool JavaScriptAccessor::Get(const CefString &name, 
        const CefRefPtr<CefV8Value> object, 
        CefRefPtr<CefV8Value> &retval, 
        CefString &exception)
    {
        printf("accessor: %s\n", name.ToString().c_str());

        auto search = _object.find(name);

        if (search == _object.end())
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
        _object[name] = value;

        return true;
    }
}
