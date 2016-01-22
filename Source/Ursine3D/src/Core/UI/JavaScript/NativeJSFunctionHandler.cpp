/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** NativeJSFunctionHandler.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "NativeJSFunctionHandler.h"

#include "NativeJSFunction.h"

#include <boost/regex.hpp>

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

            // replace namespace qualifiers with underscores
            auto name = boost::regex_replace(
                function.GetName( ), 
                boost::regex( "::" ), 
                "_" 
            );

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