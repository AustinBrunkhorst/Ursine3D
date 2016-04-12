/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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
        CefRefPtr<CefListValue> extraInfo
    ) { }

    void RenderProcessHandler::OnWebKitInitialized(void)
    {
        CefRegisterExtension( "v8/NativeBroadcastQueue", 
            "function NativeBroadcastQueue(events) {"
                "for (var i = 0, length = events.length; i < length; ++i)"
                    "NativeBroadcast.apply( null, events[ i ] );"
            "}", 
            nullptr 
        );
    }

    void RenderProcessHandler::OnBrowserCreated(
        CefRefPtr<CefBrowser> browser
    ) { }

    void RenderProcessHandler::OnBrowserDestroyed(
        CefRefPtr<CefBrowser> browser
    ) { }

    bool RenderProcessHandler::OnBeforeNavigation(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefRequest> request,
        NavigationType navigationType,
        bool isDirect
    )
    {
        // disable backspace for navigation
        if (navigationType == NAVIGATION_BACK_FORWARD)
            return true;

        return false;
    }

    void RenderProcessHandler::OnContextCreated(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefV8Context> context
    )
    {
        auto global = context->GetGlobal( );

        m_nativeBroadcaster = global->GetValue( kUINativeBroadcastFunction );

        m_globalFunctionHandler->Bind( global );

        for (auto &handler : m_nativeHandlers)
            handler->Bind( global );
    }

    void RenderProcessHandler::OnContextReleased(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefV8Context> context
    )
    {
        auto global = context->GetGlobal( );

        for (auto &handler : m_nativeHandlers)
            handler->UnBind( global );

        m_nativeBroadcaster = nullptr;
    }

    bool RenderProcessHandler::OnProcessMessageReceived(
        CefRefPtr<CefBrowser> browser,
        CefProcessId sourceProcess,
        CefRefPtr<CefProcessMessage> message
    )
    {
        if (sourceProcess == PID_BROWSER && !browser->IsLoading( ))
        {
            auto args = message->GetArgumentList( );

            // command
            switch (args->GetInt( 0 ))
            {
            case UI_CMD_BROADCAST:
                broadcast( browser, args );
                return true;
            default:
                return false;
            }
        }

        // TODO:
        return false;
    }

    void RenderProcessHandler::broadcast(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefListValue> args
    )
    {
        auto context = browser->GetMainFrame( )->GetV8Context( );

        if (!context->IsValid( ))
            return;

        context->Enter( );

        auto global = context->GetGlobal( );

        if (!m_nativeBroadcaster)
        {
            context->Exit( );

            return;
        }

        // we don't care about errors for performance reasons
        static std::string error;

        auto json = Json::parse( args->GetString( 1 ), error );

        CefV8ValueList broadcastArgs { nullptr };

        JsonSerializer::Deserialize( json, broadcastArgs[ 0 ] );

        m_nativeBroadcaster->ExecuteFunction( global, broadcastArgs );

        context->Exit( );
    }
}
