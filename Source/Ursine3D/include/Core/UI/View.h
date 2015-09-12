/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** View.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Renderer.h"

#include "MouseButton.h"

#include <cef_client.h>

namespace ursine
{
    class View
        : public Renderer
        , public CefClient
        , public CefDisplayHandler
        , public CefV8Handler
    {
        friend class UIManager;

        bool m_initialized;

        CefRefPtr<CefBrowser> m_browser;

        ////////////////////////////////////////////////////////////////////
        // Handler Getters
        ////////////////////////////////////////////////////////////////////

        CefRefPtr<CefRenderHandler> GetRenderHandler(void) override;
        CefRefPtr<CefDisplayHandler> GetDisplayHandler(void) override;

        ////////////////////////////////////////////////////////////////////
        // DisplayHandler Methods
        ////////////////////////////////////////////////////////////////////

        bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
            const CefString &message,
            const CefString &source,
            int line) override;

        ////////////////////////////////////////////////////////////////////
        // V8Handler Methods
        ////////////////////////////////////////////////////////////////////

        bool Execute(const CefString &name,
            CefRefPtr<CefV8Value> object,
            const CefV8ValueList &arguments,
            CefRefPtr<CefV8Value> &retval,
            CefString &exception) override;

        ////////////////////////////////////////////////////////////////////
        // Events
        ////////////////////////////////////////////////////////////////////

        void onKeyboard(EVENT_HANDLER(MouseManager));
        void onText(EVENT_HANDLER(MouseManager));
        void onMouseMove(EVENT_HANDLER(MouseManager));
        void onMouseButton(EVENT_HANDLER(MouseManager));
        void onMouseWheel(EVENT_HANDLER(MouseManager));

        void onWindowFocus(EVENT_HANDLER(WindowManager));
        void onWindowResize(EVENT_HANDLER(WindowManager));

        ////////////////////////////////////////////////////////////////////
        // Event Utilities
        ////////////////////////////////////////////////////////////////////

        static CefBrowserHost::MouseButtonType mapMouseButton(MouseButton button);

        static uint32 getKeyModifiers(void);

    public:
        View(void);
        ~View(void);

        void Initialize(const std::string &url, CefBrowserSettings settings);
        CefBrowser *GetBrowser(void);

        IMPLEMENT_REFCOUNTING(View);
    };
}