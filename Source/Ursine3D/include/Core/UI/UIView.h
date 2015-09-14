#pragma once

#include "Window.h"

#include "UIOpenGLRenderer.h"

#include "MouseButton.h"

#include <include/cef_client.h>
#include <include/cef_v8.h>
#include <include/cef_display_handler.h>

namespace ursine
{
    class KeyboardManager;
    class MouseManager;

    class UIView 
        : public CefClient
        , public CefDisplayHandler
        , public CefV8Handler
        , public UIOpenGLRenderer
    {
    public:
        ~UIView(void);

        void Close(void);

        const CefRect &GetViewport(void) const;
        void SetViewport(const CefRect &viewport);

        bool IsValid(void) const;

    private:
        friend class UIManager;

        Window *m_window;

        CefRefPtr<CefBrowser> m_browser;

        KeyboardManager *m_keyboardManager;
        MouseManager *m_mouseManager;

        UIView(Window *window, const CefBrowserSettings &settings, const std::string &url);

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

        void OnCursorChange(CefRefPtr<CefBrowser> browser,
            CefCursorHandle cursor,
            CursorType type,
            const CefCursorInfo &customCursorInfo) override;

        ////////////////////////////////////////////////////////////////////
        // V8Handler Methods
        ////////////////////////////////////////////////////////////////////

        bool Execute(const CefString &name,
            CefRefPtr<CefV8Value> object,
            const CefV8ValueList &arguments,
            CefRefPtr<CefV8Value> &retval,
            CefString &exception) override;

        ////////////////////////////////////////////////////////////////////
        // Event Handlers
        ////////////////////////////////////////////////////////////////////
        
        void onKeyboard(EVENT_HANDLER(MouseManager));
        void onText(EVENT_HANDLER(MouseManager));
        void onMouseMove(EVENT_HANDLER(MouseManager));
        void onMouseButton(EVENT_HANDLER(MouseManager));
        void onMouseWheel(EVENT_HANDLER(MouseManager));

        void onWindowFocus(EVENT_HANDLER(Window));

        static CefBrowserHost::MouseButtonType mapMouseButton(MouseButton button);

        uint32 getKeyModifiers(void);

        IMPLEMENT_REFCOUNTING(UIView);
    };
}