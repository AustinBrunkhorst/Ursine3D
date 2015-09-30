#include "UrsinePrecompiled.h"

#include "NativeJavaScriptHandler.h"

namespace ursine
{
    NativeJavaScriptHandler::NativeJavaScriptHandler(meta::Type classType)
        : m_classType( classType )
        , m_constructor( classType.GetConstructor( ) )
        , m_prototypeHandler( new PrototypeHandler( ) )
    {
        
    }

    void NativeJavaScriptHandler::Bind(CefRefPtr<CefV8Value> object)
    {
        auto &className = m_classType.GetName( );

        object->SetValue(
            className, 
            object->CreateFunction( className, this ),
            V8_PROPERTY_ATTRIBUTE_DONTDELETE
        );

        auto methods = m_classType.GetMethods( );

        for (auto &method : methods)
        {
            auto &methodName = method.GetName( );

            m_prototypeHandler->methods.emplace(
                std::make_pair(
                    methodName,
                    std::make_pair(
                        object->CreateFunction( methodName, m_prototypeHandler ),
                        method
                    )
                )
            );
        }
    }

    void NativeJavaScriptHandler::UnBind(CefRefPtr<CefV8Value> object)
    {
        // release function handler instances
        for (auto &method : m_prototypeHandler->methods)
            method.second.first = nullptr;
    }

    bool NativeJavaScriptHandler::Execute(
        const CefString &name, 
        CefRefPtr<CefV8Value> context, 
        const CefV8ValueList &arguments, 
        CefRefPtr<CefV8Value> &retval, 
        CefString &exception
    )
    {
        auto object = context->CreateObject( nullptr );

        object->SetUserData( new InstanceWrapper( m_constructor ) );

        for (auto &method : m_prototypeHandler->methods)
        {
            object->SetValue(
                method.first, 
                method.second.first, 
                V8_PROPERTY_ATTRIBUTE_DONTDELETE
            );
        }

        retval = object;

        return true;
    }

    bool NativeJavaScriptHandler::PrototypeHandler::Execute(
        const CefString &name, 
        CefRefPtr<CefV8Value> context, 
        const CefV8ValueList &arguments, 
        CefRefPtr<CefV8Value> &retval, CefString &exception)
    {
        auto data = context->GetUserData( );

        auto *wrapper = static_cast<InstanceWrapper *>( data.get( ) );

        auto search = methods.find( name );

        if (search == methods.end( ))
            return false;

        auto result = search->second.second.Invoke(
            wrapper->instance,
            meta::ArgumentList {
                context,
                std::move( arguments ),
                std::move( exception )
            }
        );

        retval = result.GetValue<CefRefPtr<CefV8Value>>( );

        return true;
    }

    NativeJavaScriptHandler::InstanceWrapper::InstanceWrapper(const meta::Constructor &constructor)
        : instance( constructor.Invoke( ) )
    {
        
    }
}