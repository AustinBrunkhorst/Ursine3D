/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RenderProcessHandler.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <cef_app.h>

#include "NativeJSClassHandler.h"
#include "NativeJSFunctionHandler.h"

namespace ursine
{
    class RenderProcessHandler : public CefRenderProcessHandler
    {
    protected:
        RenderProcessHandler(void);
        ~RenderProcessHandler(void);

    private:
        CefRefPtr<NativeJSFunctionHandler> m_globalFunctionHandler;

        std::vector<CefRefPtr<NativeJSClassHandler>> m_nativeHandlers;

        CefRefPtr<CefV8Value> m_nativeBroadcaster;

        void OnRenderThreadCreated(
            CefRefPtr<CefListValue> extraInfo) override;

        void OnWebKitInitialized(void) override;

        void OnBrowserCreated(CefRefPtr<CefBrowser> browser) override;

        void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) override;

        bool OnBeforeNavigation(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefRequest> request,
            NavigationType navigationType,
            bool isRedirect) override;

        void OnContextCreated(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefV8Context> context) override;

        void OnContextReleased(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefV8Context> context) override;

        bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
            CefProcessId sourceProcess,
            CefRefPtr<CefProcessMessage> message) override;

        void broadcast(CefRefPtr<CefBrowser> browser, CefRefPtr<CefListValue> args);
    };
}