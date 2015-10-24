#include "UrsinePrecompiled.h"

#include "UIView.h"

#include "KeyboardModifiers.h"
#include "KeyboardManager.h"
#include "MouseManager.h"

#if defined(PLATFORM_WINDOWS)

#include <winuser.h>

#endif

namespace ursine
{
    UIView::UIView(Window::Handle window, const CefBrowserSettings &settings, const std::string &url)
        : m_window( window )
    {
        CefWindowInfo info;

        info.SetAsWindowless( 
            static_cast<CefWindowHandle>( window->GetPlatformHandle( ) ), 
            true 
        );

        m_browser = CefBrowserHost::CreateBrowserSync(
            info,
            this,
            url,
            settings,
            nullptr 
        );

        UAssert( m_browser, "Unable to create UIView" );

        auto *app = Application::Instance;

        m_keyboardManager = GetCoreSystem( KeyboardManager );
        m_mouseManager = GetCoreSystem( MouseManager );

        m_window->Listener( this )
            .On( WINDOW_FOCUS_CHANGED, &UIView::onWindowFocus );

        m_keyboardManager->Listener( this )
            .On( KM_KEY_DOWN, &UIView::onKeyboard )
            .On( KM_KEY_UP, &UIView::onKeyboard )
            .On( KM_TEXT_INPUT, &UIView::onText );

        m_mouseManager->Listener( this )
            .On( MM_BUTTON_DOWN, &UIView::onMouseButton )
            .On( MM_BUTTON_UP, &UIView::onMouseButton )
            .On( MM_MOVE, &UIView::onMouseMove )
            .On( MM_SCROLL, &UIView::onMouseWheel );
    }

    UIView::~UIView(void)
    {
        
    }

    void UIView::Close(void)
    {
        m_window->Listener( this )
            .Off( WINDOW_FOCUS_CHANGED, &UIView::onWindowFocus );

        m_keyboardManager->Listener( this )
            .Off( KM_KEY_DOWN, &UIView::onKeyboard )
            .Off( KM_KEY_UP, &UIView::onKeyboard )
            .Off( KM_TEXT_INPUT, &UIView::onText );

        m_mouseManager->Listener( this )
            .Off( MM_BUTTON_DOWN, &UIView::onMouseButton )
            .Off( MM_BUTTON_UP, &UIView::onMouseButton )
            .Off( MM_MOVE, &UIView::onMouseMove )
            .Off( MM_SCROLL, &UIView::onMouseWheel );

        m_browser->GetHost( )->CloseBrowser( true );

        m_browser = nullptr;
    }

    const CefRect &UIView::GetViewport(void) const
    {
        URSINE_TODO( "..." );
        CefRect todo;

        return todo;
    }

    void UIView::SetViewport(const CefRect &viewport)
    {
        URSINE_TODO( "..." );
        //m_viewport = viewport;

        resize(viewport.width, viewport.height);

        m_browser->GetHost()->WasResized();
    }

    bool UIView::IsValid(void) const
    {
        return !!m_browser;
    }

    void UIView::Message(UIMessageCommand command, const std::string &target, const std::string &message, Json &data)
    {
        auto processMessage = CefProcessMessage::Create( target );

        auto args = processMessage->GetArgumentList( );

        args->SetSize( 3 );

        args->SetInt( 0, command );
        args->SetString( 1, message );
        args->SetString( 2, data.dump( ) );

        m_browser->SendProcessMessage( PID_RENDERER, processMessage );
    }

    CefRefPtr<CefRenderHandler> UIView::GetRenderHandler(void)
    {
        return this;
    }

    CefRefPtr<CefDisplayHandler> UIView::GetDisplayHandler(void)
    {
        return this;
    }

    bool UIView::OnConsoleMessage(CefRefPtr<CefBrowser> browser, const CefString &message, const CefString &source, int line)
    {

        return true;
    }

    void UIView::OnCursorChange(CefRefPtr<CefBrowser> browser, CefCursorHandle cursor, CursorType type, const CefCursorInfo &customCursorInfo)
    {
        auto handle = m_window->GetPlatformHandle( );

#if defined(PLATFORM_WINDOWS)

        SetClassLongPtr( 
            static_cast<HWND>( handle ), 
            GCLP_HCURSOR,
            static_cast<LONG>( reinterpret_cast<LONG_PTR>( cursor ) )
        );

        SetCursor( cursor );
#endif
    }

    void UIView::onKeyboard(EVENT_HANDLER(KeyboardManager))
    {
        EVENT_ATTRS(KeyboardManager, KeyboardKeyArgs);

        CefKeyEvent e;

        e.type = args->state ? KEYEVENT_RAWKEYDOWN : KEYEVENT_KEYUP;

        e.modifiers = getKeyModifiers( );

        e.is_system_key = false;

        e.windows_key_code = args->key;
        e.native_key_code = KeyboardManager::KeyToScan( args->key );

        m_browser->GetHost( )->SendKeyEvent( e );

        // handle return key (not handled by text input)
        if (args->state && args->key == KEY_RETURN)
        {
            e.type = KEYEVENT_CHAR;

            e.character = e.unmodified_character = '\n';

            m_browser->GetHost( )->SendKeyEvent( e );
        }
    }

    void UIView::onText(EVENT_HANDLER(KeyboardManager))
    {
        EVENT_ATTRS(KeyboardManager, KeyboardTextArgs);

        CefKeyEvent e;

        e.type = KEYEVENT_CHAR;

        e.modifiers = getKeyModifiers( );

        e.is_system_key = false;

        e.character = e.unmodified_character =
            e.native_key_code = e.windows_key_code = args->text[ 0 ];

        m_browser->GetHost( )->SendKeyEvent( e );
    }

    void UIView::onMouseMove(EVENT_HANDLER(MouseManager))
    {
        EVENT_ATTRS(MouseManager, MouseMoveArgs);

        CefMouseEvent e;

        e.modifiers = getKeyModifiers( );

        if (m_mouseManager->IsButtonDown( MBTN_LEFT ))
            utils::FlagSet( e.modifiers, EVENTFLAG_LEFT_MOUSE_BUTTON );

        if (m_mouseManager->IsButtonDown( MBTN_MIDDLE ))
            utils::FlagSet( e.modifiers, EVENTFLAG_MIDDLE_MOUSE_BUTTON );

        if (m_mouseManager->IsButtonDown( MBTN_RIGHT ))
            utils::FlagSet( e.modifiers, EVENTFLAG_RIGHT_MOUSE_BUTTON );

        URSINE_TODO( "..." );
        CefRect m_viewport { 0, 0, 0, 0 };

        e.x = static_cast<int>( args->position.X( ) - m_viewport.x );
        e.y = static_cast<int>( args->position.Y( ) - m_viewport.y );

        m_browser->GetHost( )->SendMouseMoveEvent( e, false );
    }

    void UIView::onMouseButton(EVENT_HANDLER(MouseManager))
    {
        EVENT_ATTRS(MouseManager, MouseButtonArgs);

        CefMouseEvent e;

        e.modifiers = getKeyModifiers( );

        if (m_mouseManager->IsButtonDown( MBTN_LEFT ))
            utils::FlagSet( e.modifiers, EVENTFLAG_LEFT_MOUSE_BUTTON );

        if (m_mouseManager->IsButtonDown( MBTN_MIDDLE ))
            utils::FlagSet( e.modifiers, EVENTFLAG_MIDDLE_MOUSE_BUTTON );

        if (m_mouseManager->IsButtonDown( MBTN_RIGHT ))
            utils::FlagSet( e.modifiers, EVENTFLAG_RIGHT_MOUSE_BUTTON );

        URSINE_TODO( "..." );
        CefRect m_viewport { 0, 0, 0, 0 };

        e.x = static_cast<int>( args->position.X( ) - m_viewport.x );
        e.y = static_cast<int>( args->position.Y( ) - m_viewport.y );

        m_browser->GetHost( )->SendMouseClickEvent(
            e,
            mapMouseButton( args->button ),
            !args->state,
            args->clicks
        );
    }

    void UIView::onMouseWheel(EVENT_HANDLER(MouseManager))
    {
        EVENT_ATTRS(MouseManager, MouseScrollArgs);

        // TODO:
        const uint32 speed = 60;

        CefMouseEvent e;

        e.modifiers = getKeyModifiers( );

        if (m_mouseManager->IsButtonDown( MBTN_LEFT ))
            utils::FlagSet( e.modifiers, EVENTFLAG_LEFT_MOUSE_BUTTON );

        if (m_mouseManager->IsButtonDown( MBTN_MIDDLE ))
            utils::FlagSet( e.modifiers, EVENTFLAG_MIDDLE_MOUSE_BUTTON );

        if (m_mouseManager->IsButtonDown( MBTN_RIGHT ))
            utils::FlagSet( e.modifiers, EVENTFLAG_RIGHT_MOUSE_BUTTON );

        auto &position = m_mouseManager->GetPosition( );

        URSINE_TODO( "..." );
        CefRect m_viewport { 0, 0, 0, 0 };

        e.x = static_cast<int>( position.X( ) - m_viewport.x );
        e.y = static_cast<int>( position.Y( ) - m_viewport.y );

        m_browser->GetHost( )->SendMouseWheelEvent( 
            e,
            static_cast<int>( args->delta.X( ) * speed ),
            static_cast<int>( args->delta.Y( ) * speed ) 
        );
    }

    void UIView::onWindowFocus(EVENT_HANDLER(Window))
    {
        EVENT_ATTRS(Window, WindowFocusArgs);

        m_browser->GetHost( )->SendFocusEvent( args->focused );
    }

    CefBrowserHost::MouseButtonType UIView::mapMouseButton(MouseButton button)
    {
        switch (button)
        {
            case MBTN_LEFT: return MBT_LEFT;
            case MBTN_MIDDLE: return MBT_MIDDLE;
            case MBTN_RIGHT: return MBT_RIGHT;

                // default to left mouse button
            default: return MBT_LEFT;
        }
    }

    uint32 UIView::getKeyModifiers(void)
    {
        auto state = m_keyboardManager->GetModifiers( );

        uint32 modifiers = 0;

        if (state & KMD_CTRL)
            utils::FlagSet( modifiers, EVENTFLAG_CONTROL_DOWN );

        if (state & KMD_SHIFT)
            utils::FlagSet( modifiers, EVENTFLAG_SHIFT_DOWN );

        if (state & KMD_ALT)
            utils::FlagSet( modifiers, EVENTFLAG_ALT_DOWN );

        if (state & KMD_NUM)
            utils::FlagSet( modifiers, EVENTFLAG_NUM_LOCK_ON );

        if (state & KMD_NUM)
            utils::FlagSet( modifiers, EVENTFLAG_CAPS_LOCK_ON );

        if (utils::IsFlagSet( state, KMD_LCTRL ) ||
            utils::IsFlagSet( state, KMD_LALT ) ||
            utils::IsFlagSet( state, KMD_LSHIFT ))
        {
            utils::FlagSet( modifiers, EVENTFLAG_IS_LEFT );
        }

        if (utils::IsFlagSet( state, KMD_RCTRL ) ||
            utils::IsFlagSet( state, KMD_RALT ) ||
            utils::IsFlagSet( state, KMD_RSHIFT ))
        {
            utils::FlagSet( modifiers, EVENTFLAG_IS_RIGHT );
        }

        return modifiers;
    }
}