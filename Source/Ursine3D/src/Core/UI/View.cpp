/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** View.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


#include "UrsinePrecompiled.h"

#include "View.h"
#include "UIConfig.h"
#include "Core.h"
#include "UIManager.h"
#include "BrowserMessageHandler.h"

#include "WindowManager.h"
#include "KeyboardManager.h"
#include "MouseManager.h"

#include "UrsineConsole.h"

#include <boost/filesystem/path.hpp>

#ifdef CONFIG_DEBUG
#define ENABLE_DEVTOOLS_LAUNCH
#ifdef PLATFORM_WINDOWS
// ShellExecute()
#pragma comment(lib, "shell32.lib")
#endif
#endif

namespace ursine
{
    View::View(void)
        : m_initialized( false ) { }

    View::~View(void)
    {
        if (!m_initialized)
            return;

        URSINE_TODO("...");
        /*gKeyboardManager->Listener( this )
            .Off( KM_KEY_DOWN, &View::onKeyboard )
            .Off( KM_KEY_UP, &View::onKeyboard )
            .Off( KM_TEXT_INPUT, &View::onText );

        gMouseManager->Listener( this )
            .Off( MM_BUTTON_DOWN, &View::onMouseButton )
            .Off( MM_BUTTON_UP, &View::onMouseButton )
            .Off( MM_MOVE, &View::onMouseMove )
            .Off( MM_SCROLL, &View::onMouseWheel );

        gWindowManager->Listener( this )
            .Off( WM_FOCUS_CHANGED, &View::onWindowFocus )
            .Off( WM_RESIZE, &View::onWindowResize );*/
    }

    void View::Initialize(const std::string &url, CefBrowserSettings settings)
    {
        if (m_initialized)
            return;

        m_initialized = true;

        CefWindowInfo info;

        URSINE_TODO("...");
        /*info.SetAsOffScreen(
            static_cast<HWND>( gWindowManager->GetPlatformHandle( ) ) );*/

        info.SetTransparentPainting( true );

        m_browser = CefBrowserHost::CreateBrowserSync(
            info,
            this,
            url,
            settings,
            nullptr );

        UAssert(m_browser, "Unable to create browser for View.");

        URSINE_TODO("...");
        /*gKeyboardManager->Listener( this )
            .On( KM_KEY_DOWN, &View::onKeyboard )
            .On( KM_KEY_UP, &View::onKeyboard )
            .On( KM_TEXT_INPUT, &View::onText );

        gMouseManager->Listener( this )
            .On( MM_BUTTON_DOWN, &View::onMouseButton )
            .On( MM_BUTTON_UP, &View::onMouseButton )
            .On( MM_MOVE, &View::onMouseMove )
            .On( MM_SCROLL, &View::onMouseWheel );

        gWindowManager->Listener( this )
            .On( WM_FOCUS_CHANGED, &View::onWindowFocus )
            .On( WM_RESIZE, &View::onWindowResize );*/
    }

    ////////////////////////////////////////////////////////////////////////
    // Handler Getters
    ////////////////////////////////////////////////////////////////////////

    CefRefPtr<CefRenderHandler> View::GetRenderHandler(void)
    {
        return this;
    }

    CefRefPtr<CefDisplayHandler> View::GetDisplayHandler(void)
    {
        return this;
    }

    ////////////////////////////////////////////////////////////////////////
    // DisplayHandler Methods
    ////////////////////////////////////////////////////////////////////////

    bool View::OnConsoleMessage(CefRefPtr<CefBrowser> browser,
        const CefString &message,
        const CefString &source,
        int line)
    {
        SetConsoleColor( CC_TEXT_BRIGHT_GREEN );

        printf( "javascript " );

        SetConsoleColor( CC_TEXT_BRIGHT_YELLOW );

        auto filename = boost::filesystem::path( source ).filename( ).string( );

        if (!filename.empty( ))
        {
            printf( "[" );

            SetConsoleColor( CC_TEXT_YELLOW );

            printf( "%s", filename.c_str( ) );

            SetConsoleColor( CC_TEXT_WHITE );

            printf( " %i", line );

            SetConsoleColor( CC_TEXT_BRIGHT_YELLOW );

            printf( "]" );
        }

        SetConsoleColor( CC_TEXT_WHITE );

        printf( "\n> " );

        SetConsoleColor( CC_TEXT_BRIGHT_WHITE );

        printf( "%s\n\n", message.ToString( ).c_str( ) );

        SetConsoleColor( CC_TEXT_WHITE );

        return false;
    }

    ////////////////////////////////////////////////////////////////////////
    // V8Handler Methods
    ////////////////////////////////////////////////////////////////////////

    bool View::Execute(const CefString &name,
        CefRefPtr<CefV8Value> object,
        const CefV8ValueList &arguments,
        CefRefPtr<CefV8Value> &retval,
        CefString &exception)
    {
        return true;
    }

    CefBrowser *View::GetBrowser(void)
    {
        return m_browser;
    }

    ////////////////////////////////////////////////////////////////////////
    // Events
    ////////////////////////////////////////////////////////////////////////

    void View::onKeyboard(EVENT_HANDLER(KeyboardManager))
    {
        EVENT_ATTRS(KeyboardManager, KeyboardKeyArgs);

        CefKeyEvent e;

        e.type = args->state ? KEYEVENT_RAWKEYDOWN : KEYEVENT_KEYUP;

        e.modifiers = getKeyModifiers( );

        e.is_system_key = false;

        e.windows_key_code = args->key;
        e.native_key_code = KeyboardManager::KeyToScan( args->key );

        m_browser->GetHost( )->SendKeyEvent( e );

#ifdef ENABLE_DEVTOOLS_LAUNCH
        // CTRL + SHIFT + J
        if (args->state &&
            args->repeat == 0 &&
            utils::IsFlagSet( e.modifiers, EVENTFLAG_CONTROL_DOWN | EVENTFLAG_SHIFT_DOWN ) &&
            e.windows_key_code == KEY_J)
        {
            std::string debug_url( "http://localhost:" );

            debug_url += std::to_string( UIManager::REMOTE_DEBUGGING_PORT );

#ifdef PLATFORM_WINDOWS
            ShellExecute( nullptr, "open", debug_url.c_str( ), nullptr, nullptr, SW_SHOWNORMAL );
#endif
        }
#endif

        // handle return key (not handled by text input)
        if (args->state && args->key == KEY_RETURN)
        {
            e.type = KEYEVENT_CHAR;

            e.character = e.unmodified_character = '\n';

            m_browser->GetHost( )->SendKeyEvent( e );
        }
    }

    void View::onText(EVENT_HANDLER(KeyboardManager))
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

    void View::onMouseMove(EVENT_HANDLER(MouseManager))
    {
        EVENT_ATTRS(MouseManager, MouseMoveArgs);

        CefMouseEvent e;

        e.modifiers = getKeyModifiers( );

        URSINE_TODO("...");
        /*if (gMouseManager->IsButtonDown( MBTN_LEFT ))
            utils::FlagSet( e.modifiers, EVENTFLAG_LEFT_MOUSE_BUTTON );

        if (gMouseManager->IsButtonDown( MBTN_MIDDLE ))
            utils::FlagSet( e.modifiers, EVENTFLAG_MIDDLE_MOUSE_BUTTON );

        if (gMouseManager->IsButtonDown( MBTN_RIGHT ))
            utils::FlagSet( e.modifiers, EVENTFLAG_RIGHT_MOUSE_BUTTON );*/

        e.x = static_cast<int>( args->position.X( ) );
        e.y = static_cast<int>( args->position.Y( ) );

        m_browser->GetHost( )->SendMouseMoveEvent( e, false );
    }

    void View::onMouseButton(EVENT_HANDLER(MouseManager))
    {
        EVENT_ATTRS(MouseManager, MouseButtonArgs);

        CefMouseEvent e;

        e.modifiers = getKeyModifiers( );

        URSINE_TODO("...");
        /* if (gMouseManager->IsButtonDown( MBTN_LEFT ))
            utils::FlagSet( e.modifiers, EVENTFLAG_LEFT_MOUSE_BUTTON );

        if (gMouseManager->IsButtonDown( MBTN_MIDDLE ))
            utils::FlagSet( e.modifiers, EVENTFLAG_MIDDLE_MOUSE_BUTTON );

        if (gMouseManager->IsButtonDown( MBTN_RIGHT ))
            utils::FlagSet( e.modifiers, EVENTFLAG_RIGHT_MOUSE_BUTTON );*/

        e.x = static_cast<int>( args->position.X( ) );
        e.y = static_cast<int>( args->position.Y( ) );

        m_browser->GetHost( )->SendMouseClickEvent( e,
            mapMouseButton( args->button ),
            !args->state,
            args->clicks );
    }

    void View::onMouseWheel(EVENT_HANDLER(MouseManager))
    {
        EVENT_ATTRS(MouseManager, MouseScrollArgs);

        URSINE_TODO("...");
        /*// TODO:
        const uint32 speed = 60;

        CefMouseEvent e;

        e.modifiers = getKeyModifiers( );

        if (gMouseManager->IsButtonDown( MBTN_LEFT ))
            utils::FlagSet( e.modifiers, EVENTFLAG_LEFT_MOUSE_BUTTON );

        if (gMouseManager->IsButtonDown( MBTN_MIDDLE ))
            utils::FlagSet( e.modifiers, EVENTFLAG_MIDDLE_MOUSE_BUTTON );

        if (gMouseManager->IsButtonDown( MBTN_RIGHT ))
            utils::FlagSet( e.modifiers, EVENTFLAG_RIGHT_MOUSE_BUTTON );

        auto &position = gMouseManager->GetPosition( );

        e.x = static_cast<int>( position.X() );
        e.y = static_cast<int>( position.Y() );

        m_browser->GetHost( )->SendMouseWheelEvent( e,
            static_cast<int>( args->delta.X() * speed ),
            static_cast<int>( args->delta.Y() * speed ) );*/
    }

    inline void View::onWindowFocus(EVENT_HANDLER(WindowManager))
    {
        EVENT_ATTRS(WindowManager, WindowFocusArgs);

        m_browser->GetHost( )->SendFocusEvent( args->focused );
    }

    inline void View::onWindowResize(EVENT_HANDLER(WindowManager))
    {
        m_browser->GetHost( )->WasResized( );
    }

    ////////////////////////////////////////////////////////////////////////
    // Event Utilities
    ////////////////////////////////////////////////////////////////////////

    inline CefBrowserHost::MouseButtonType View::mapMouseButton(MouseButton button)
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

    uint32 View::getKeyModifiers(void)
    {
        URSINE_TODO("...");
        KeyboardModifiers state;// = gKeyboardManager->GetModifiers();

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
