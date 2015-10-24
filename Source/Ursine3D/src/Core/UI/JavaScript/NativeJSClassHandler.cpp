#include "UrsinePrecompiled.h"

#include "NativeJSClassHandler.h"

namespace ursine
{
    NativeJSClassHandler::NativeJSClassHandler(meta::Type classType)
        : m_classType( classType )
        , m_constructor( classType.GetDynamicConstructor( { 
            typeof( CefRefPtr<CefV8Value> ), 
            typeof( const CefV8ValueList& ),
            typeof( CefString& )
        } ) )
        , m_prototypeHandler( new PrototypeHandler( ) )
    {
        UAssert( m_constructor.IsValid( ),
            "Native JS Class '%s' does not have a proper constructor.",
            classType.GetName( ).c_str( )
        );
    }

    void NativeJSClassHandler::Bind(CefRefPtr<CefV8Value> object)
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

    void NativeJSClassHandler::UnBind(CefRefPtr<CefV8Value> object)
    {
        // release function handler instances
        for (auto &method : m_prototypeHandler->methods)
            method.second.first = nullptr;
    }

    bool NativeJSClassHandler::Execute(
        const CefString &name, 
        CefRefPtr<CefV8Value> context, 
        const CefV8ValueList &arguments, 
        CefRefPtr<CefV8Value> &retval, 
        CefString &exception
    )
    {
        auto object = context->CreateObject( nullptr );

        object->SetUserData( 
            new InstanceWrapper( m_constructor, context, arguments, exception )
        );

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

    bool NativeJSClassHandler::PrototypeHandler::Execute(
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

    NativeJSClassHandler::InstanceWrapper::InstanceWrapper(
        const meta::Constructor &constructor, 
        CefRefPtr<CefV8Value> context,
        const CefV8ValueList &arguments,
        CefString &exception
    )
        : instance( 
            constructor.Invoke( context, std::move( arguments ), std::move( exception ) )
        )
    {
        
    }
}