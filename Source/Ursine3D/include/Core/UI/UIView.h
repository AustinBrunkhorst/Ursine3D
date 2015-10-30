#pragma once

#include "UIConfig.h"

#include "Window.h"

#if defined(URSINE_GRAPHICS_OPENGL)

#include "UIOpenGLRenderer.h"

#define UIRendererType UIOpenGLRenderer

#elif defined(URSINE_GRAPHICS_DIRECTX)

#include "UIDirectXRenderer.h"

#define UIRendererType UIDirectXRenderer

#endif

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
        , public UIRendererType
    {
    public:
        typedef CefRefPtr<UIView> Handle;

        ~UIView(void);

        CefRefPtr<CefBrowser> GetBrowser(void) const;

        void Close(void);

        const CefRect &GetViewport(void) const;
        void SetViewport(const CefRect &viewport);

        bool IsValid(void) const;

        void Message(UIMessageCommand command, const std::string &target, const std::string &message, Json &data);

    private:
        friend class UIManager;

        Window::Handle m_window;

        CefRefPtr<CefBrowser> m_browser;

        KeyboardManager *m_keyboardManager;
        MouseManager *m_mouseManager;

        UIView(Window::Handle window, const CefBrowserSettings &settings, const std::string &url);

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