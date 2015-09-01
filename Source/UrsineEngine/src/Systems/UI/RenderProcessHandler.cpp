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
#include "UIConfig.h"

namespace ursine
{
    namespace ui
    {
        RenderProcessHandler::RenderProcessHandler(void) { }

        RenderProcessHandler::~RenderProcessHandler(void) { }

        void RenderProcessHandler::OnRenderThreadCreated(
            CefRefPtr<CefListValue> extra_info) { }

        void RenderProcessHandler::OnWebKitInitialized(void)
        {
            m_extensions.emplace_back( new GameExtension( ) );
            
            for (auto extension : m_extensions)
                extension->Register( );
        }

        void RenderProcessHandler::OnBrowserCreated(
            CefRefPtr<CefBrowser> browser) { }

        void RenderProcessHandler::OnBrowserDestroyed(
            CefRefPtr<CefBrowser> browser) { }

        bool RenderProcessHandler::OnBeforeNavigation(
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefRequest> request,
            NavigationType navigation_type,
            bool is_redirect)
        {
            return false;
        }

        void RenderProcessHandler::OnContextCreated(
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefV8Context> context) { }

        void RenderProcessHandler::OnContextReleased(
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefV8Context> context) { }

        bool RenderProcessHandler::OnProcessMessageReceived(
            CefRefPtr<CefBrowser> browser,
            CefProcessId source_process,
            CefRefPtr<CefProcessMessage> message)
        {
            if (source_process == PID_BROWSER)
            {
                auto context = browser->GetMainFrame( )->GetV8Context( );

                if (context->IsValid( ))
                {
                    context->Enter( );

                    auto window = context->GetGlobal( );

                    auto game = window->GetValue( kGameInstanceName );

                    if (game->IsObject( ))
                    {
                        auto trigger = game->GetValue( kGameFnTrigger );

                        if (trigger->IsFunction( ))
                        {
                            CefV8ValueList arguments( 2 );

                            arguments[ 0 ] = CefV8Value::CreateString( message->GetName( ) );
                            arguments[ 1 ] = CefV8Value::CreateString( message->GetArgumentList( )->GetString( 0 ) );

                            trigger->ExecuteFunction( window, arguments );
                        }
                    }

                    context->Exit( );
                }
            }

            return true;
        }
    }
}
