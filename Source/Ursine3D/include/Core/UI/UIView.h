/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UIView.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "UIConfig.h"
#include "UIViewEvent.h"

#include "Window.h"

#if defined(URSINE_GRAPHICS_OPENGL)

#include "UIOpenGLRenderer.h"

#define UIRendererType UIOpenGLRenderer

#elif defined(URSINE_GRAPHICS_DIRECTX)

#include "UIDirectXRenderer.h"

#define UIRendererType UIDirectXRenderer

#endif

#include "MouseButton.h"

#include <cef_client.h>
#include <cef_v8.h>
#include <cef_display_handler.h>

namespace ursine
{
    class KeyboardManager;
    class MouseManager;

    class UIView 
        : public EventDispatcher<UIViewEventType>
        , public CefClient
        , public CefLifeSpanHandler
        , public CefDisplayHandler
        , public CefLoadHandler
        , public UIRendererType
    {
    public:
        typedef CefRefPtr<UIView> Handle;

        ~UIView(void);

        static void Message(
            CefRefPtr<CefBrowser> browser,
            UIMessageCommand command,
            const std::string &target,
            const std::string &message,
            const Json &data
        );

        CefRefPtr<CefBrowser> GetBrowser(void) const;

        void Close(void);

        const CefRect &GetViewport(void) const;
        void SetViewport(const CefRect &viewport);

        bool IsValid(void) const;

        void Message(
            UIMessageCommand command, 
            const std::string &target, 
            const std::string &message, 
            const Json &data
        );

    private:
        friend class UIManager;

        struct QueuedMessage
        {
            UIMessageCommand command;
            std::string target;
            std::string message;
            Json data;
        };

        std::vector<QueuedMessage> m_preLoadQueue;

        CefRect m_viewport;
        Window::Handle m_window;

        CefRefPtr<CefBrowser> m_browser;

        KeyboardManager *m_keyboardManager;
        MouseManager *m_mouseManager;

        UIView(
            Window::Handle window, 
            const CefBrowserSettings &settings, 
            const std::string &url
        );

        ////////////////////////////////////////////////////////////////////
        // Handler Getters
        ////////////////////////////////////////////////////////////////////

        CefRefPtr<CefRenderHandler> GetRenderHandler(void) override;
        CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler(void) override;
        CefRefPtr<CefDisplayHandler> GetDisplayHandler(void) override;
        CefRefPtr<CefLoadHandler> GetLoadHandler(void) override;

        ////////////////////////////////////////////////////////////////////
        // LifeSpanHandler Methods
        ////////////////////////////////////////////////////////////////////

        bool OnBeforePopup(
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            const CefString &targetURL,
            const CefString &targetFrameName,
            WindowOpenDisposition targetDisposition,
            bool userGesture,
            const CefPopupFeatures &popupFeatures,
            CefWindowInfo &windowInfo,
            CefRefPtr<CefClient> &client,
            CefBrowserSettings &settings,
            bool *noJavaScriptAccess
        ) override;

        ////////////////////////////////////////////////////////////////////
        // DisplayHandler Methods
        ////////////////////////////////////////////////////////////////////

        bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
            const CefString &message,
            const CefString &source,
            int line
        ) override;

        void OnCursorChange(CefRefPtr<CefBrowser> browser,
            CefCursorHandle cursor,
            CursorType type,
            const CefCursorInfo &customCursorInfo
        ) override;


        ////////////////////////////////////////////////////////////////////
        // LoadHandler Methods
        ////////////////////////////////////////////////////////////////////

        void OnLoadEnd(
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            int httpStatusCode
        );

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