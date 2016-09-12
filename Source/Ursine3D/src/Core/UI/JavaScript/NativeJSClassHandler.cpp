/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** NativeJSClassHandler.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "NativeJSClassHandler.h"

#include "NativeJSClass.h"

namespace ursine
{
    NativeJSClassHandler::NativeJSClassHandler(meta::Type classType)
        : m_classType( classType )
        , m_constructor( classType.GetDynamicConstructor( { 
            typeof( CefRefPtr<CefV8Value> ), 
            typeof( const CefV8ValueList& ),
            typeof( CefString& ),
            typeof( CefRefPtr<CefV8Value> )
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
        auto className = m_classType.GetName( );

        object->SetValue(
            className, 
            object->CreateFunction( className, this ),
            V8_PROPERTY_ATTRIBUTE_DONTDELETE
        );

        auto methods = m_classType.GetMethods( );

        for (auto &method : methods)
        {
            auto &methodName = method.GetName( );

            auto handler = object->CreateFunction( methodName, m_prototypeHandler );

            auto result = m_prototypeHandler->methods.emplace(
                make_pair( methodName, std::make_pair( handler, method ) )
            );

            // if it already exists, update the handler
            if (!result.second)
                result.first->second.first = handler;
        }
    }

    void NativeJSClassHandler::UnBind(CefRefPtr<CefV8Value> object)
    {
        m_instances.clear( );

        // release function handler instances
        for (auto &method : m_prototypeHandler->methods)
            method.second.first = nullptr;

        object->DeleteValue( m_classType.GetName( ) );
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

        auto wrapper = new InstanceWrapper( 
            m_constructor, 
            context, 
            arguments, 
            exception, 
            object 
        );

        m_instances.push_back( wrapper );

        object->SetUserData( wrapper );

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
        CefRefPtr<CefV8Value> &retval, 
        CefString &exception
    )
    {
        auto search = methods.find( name );

        if (search == methods.end( ))
            return false;

        auto data = context->GetUserData( );

        auto *wrapper = static_cast<InstanceWrapper*>( data.get( ) );

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
        CefString &exception,
        CefRefPtr<CefV8Value> thisContext
    )
        : instance( 
            constructor.Invoke( context, std::move( arguments ), std::move( exception ), thisContext )
        )
    {
        
    }

    NativeJSClassHandler::InstanceWrapper::~InstanceWrapper(void)
    {
        delete &instance.GetValue<NativeJSClass>( );
    }
}