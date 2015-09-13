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
    RenderProcessHandler::RenderProcessHandler(void) { }

    RenderProcessHandler::~RenderProcessHandler(void) { }

    void RenderProcessHandler::OnRenderThreadCreated(
        CefRefPtr<CefListValue> extraInfo) { }

    void RenderProcessHandler::OnWebKitInitialized(void)
    {
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
        NavigationType navigationType,
        bool isDirect)
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
        CefProcessId sourceProcess,
        CefRefPtr<CefProcessMessage> message)
    {
        if (sourceProcess == PID_BROWSER)
        {
            auto context = browser->GetMainFrame( )->GetV8Context( );

            if (context->IsValid( ))
            {
                context->Enter( );

               // TODO:

                context->Exit( );
            }
        }

        return true;
    }
}
