/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

#include "NativeJavaScriptHandler.h"

namespace ursine
{
    class RenderProcessHandler : public CefRenderProcessHandler
    {
    protected:
        RenderProcessHandler(void);
        ~RenderProcessHandler(void);

    private:
        std::vector<CefRefPtr<NativeJavaScriptHandler>> m_nativeHandlers;

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

        bool initNativeBroadcaster(CefRefPtr<CefV8Value> global);

        void broadcast(CefRefPtr<CefBrowser> browser, const std::string &target, CefRefPtr<CefListValue> args);
    };
}