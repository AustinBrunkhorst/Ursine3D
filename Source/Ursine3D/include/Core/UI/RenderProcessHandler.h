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

#include "JavascriptExtension.h"

namespace ursine
{
    class RenderProcessHandler : public CefRenderProcessHandler
    {
    protected:
        RenderProcessHandler(void);
        ~RenderProcessHandler(void);

    private:
        std::vector<CefRefPtr<JavaScriptExtension>> m_extensions;

        void OnRenderThreadCreated(
            CefRefPtr<CefListValue> extra_info) override;

        void OnWebKitInitialized(void) override;

        void OnBrowserCreated(CefRefPtr<CefBrowser> browser) override;

        void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) override;

        bool OnBeforeNavigation(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefRequest> request,
            NavigationType navigation_type,
            bool is_redirect) override;

        void OnContextCreated(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefV8Context> context) override;

        void OnContextReleased(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefV8Context> context) override;

        bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
            CefProcessId source_process,
            CefRefPtr<CefProcessMessage> message) override;
    };
}