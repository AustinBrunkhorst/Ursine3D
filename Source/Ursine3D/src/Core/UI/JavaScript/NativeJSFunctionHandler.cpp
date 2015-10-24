#include "UrsinePrecompiled.h"

#include "NativeJSFunctionHandler.h"

#include "NativeJSFunction.h"

namespace ursine 
{
    void NativeJSFunctionHandler::Bind(CefRefPtr<CefV8Value> context)
    {
        auto globalFunctions = meta::Type::GetGlobalFunctions( );

        auto exposeProperty = typeof( ExposeJavaScript );

        for (auto &function : globalFunctions)
        {
            // not exposed
            if (!function.GetMeta( ).GetProperty( exposeProperty ).IsValid( ))
                continue;

            auto &name = function.GetName( );

            m_functions.emplace( name, function );

            context->SetValue( 
                name, 
                CefV8Value::CreateFunction( name, this ),
                V8_PROPERTY_ATTRIBUTE_DONTDELETE
            );
        }
    }

    bool NativeJSFunctionHandler::Execute(
        const CefString &name, 
        CefRefPtr<CefV8Value> object, 
        const CefV8ValueList &arguments, 
        CefRefPtr<CefV8Value> &retval, 
        CefString &exception
    )
    {
        meta::ArgumentList fnArgs { 
            object, 
            std::move( arguments ),
            std::move( exception )
        };

        auto &function = m_functions[ name ];

        retval = function.Invoke( fnArgs ).GetValue<CefRefPtr<CefV8Value>>( );

        return true;
    }
}