/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RenderProcessHandler.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "RenderProcessHandler.h"

#include "NativeJSClass.h"

#include "UIConfig.h"
#include "JSConfig.h"

namespace ursine
{
    RenderProcessHandler::RenderProcessHandler(void)
        : m_globalFunctionHandler( new NativeJSFunctionHandler( ) )
        , m_nativeBroadcaster( nullptr )
    {
        auto exposedType = typeof( NativeJSClass );

        for (auto &exposed : exposedType.GetDerivedClasses( ))
            m_nativeHandlers.emplace_back( new NativeJSClassHandler( exposed ) );
    }

    RenderProcessHandler::~RenderProcessHandler(void) { }

    void RenderProcessHandler::OnRenderThreadCreated(
        CefRefPtr<CefListValue> extraInfo) { }

    void RenderProcessHandler::OnWebKitInitialized(void)
    {
        
    }

    void RenderProcessHandler::OnBrowserCreated(
        CefRefPtr<CefBrowser> browser) { }

    void RenderProcessHandler::OnBrowserDestroyed(
        CefRefPtr<CefBrowser> browser) { }

    bool RenderProcessHandler::OnBeforeNavigation(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefRequest> request,
        NavigationType navigationType,
        bool isDirect)
    {
        return false;
    }

    void RenderProcessHandler::OnContextCreated(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefV8Context> context)
    {
        auto global = context->GetGlobal( );

        m_globalFunctionHandler->Bind( global );

        for (auto &handler : m_nativeHandlers)
            handler->Bind( global );
    }

    void RenderProcessHandler::OnContextReleased(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefV8Context> context)
    {
        auto global = context->GetGlobal( );

        for (auto &handler : m_nativeHandlers)
            handler->UnBind( global );

        m_nativeBroadcaster = nullptr;
    }

    bool RenderProcessHandler::OnProcessMessageReceived(
        CefRefPtr<CefBrowser> browser,
        CefProcessId sourceProcess,
        CefRefPtr<CefProcessMessage> message)
    {
        if (sourceProcess == PID_BROWSER && !browser->IsLoading( ))
        {
            auto args = message->GetArgumentList( );

            // command
            switch (args->GetInt( 0 ))
            {
            case UI_CMD_BROADCAST:
                broadcast( browser, message->GetName( ), args );
                return true;
            default:
                return false;
            }
        }

        // TODO:
        return false;
    }

    bool RenderProcessHandler::initNativeBroadcaster(CefRefPtr<CefV8Value> global)
    {
        m_nativeBroadcaster = global->GetValue( kNativeBroadcastFunction );

        return m_nativeBroadcaster != nullptr;
    }

    void RenderProcessHandler::broadcast(
        CefRefPtr<CefBrowser> browser, 
        const std::string &target, 
        CefRefPtr<CefListValue> args
    )
    {
        auto context = browser->GetMainFrame( )->GetV8Context( );

        if (!context->IsValid( ))
            return;

        context->Enter( );

        auto global = context->GetGlobal( );

        if (!m_nativeBroadcaster && !initNativeBroadcaster( global ))
        {
            context->Exit( );

            return;
        }

        static std::string error;

        auto json = Json::parse( args->GetString( 2 ), error );

        CefV8ValueList broadcastArgs {
            CefV8Value::CreateString( target ),
            CefV8Value::CreateString( args->GetString( 1 ) ),
            nullptr
        };

        JsonSerializer::Deserialize( json, broadcastArgs[ 2 ] );

        m_nativeBroadcaster->ExecuteFunction( global, broadcastArgs );

        context->Exit( );
    }
}
