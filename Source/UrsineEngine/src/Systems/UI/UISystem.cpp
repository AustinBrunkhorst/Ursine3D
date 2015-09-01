/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UISystem.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UISystem.h"
#include "UIConfig.h"

#include "GamepadManager.h"
#include "KeyboardManager.h"
#include "MouseManager.h"

namespace ursine
{
    namespace UI
    {
        namespace
        {
            const char * const kEventGamepadDown = "gp-btn-down";
            const char * const kEventGamepadUp = "gp-btn-up";

            const char * const kEventGamepadConnected = "gp-connected";
            const char * const kEventGamepadDisconnected = "gp-disconnected";

            const char * const kEventKeyboardKeyDown = "kb-key-down";
            const char * const kEventKeyboardKeyUp = "kb-key-up";
        }

        UISystem *gUISystem = nullptr;

        UISystem::UISystem(const std::string &path)
            : EventDispatcher( this )
            , m_core( new Core )
            , m_view( new View )
        {
            gUISystem = this;

#ifdef PLATFORM_WINDOWS
            CefMainArgs main_args( GetModuleHandle( nullptr ) );
#else
            Application *instance = Application::Instance();

            CefMainArgs main_args(instance->GetArgC(), instance->GetArgV());
#endif
            CefSettings settings;

            settings.no_sandbox = true;
            settings.ignore_certificate_errors = true;
            settings.command_line_args_disabled = true;

            settings.single_process = true;
            settings.multi_threaded_message_loop = false;

#ifdef CONFIG_DEBUG
            settings.log_severity = LOGSEVERITY_WARNING;
            settings.remote_debugging_port = REMOTE_DEBUGGING_PORT;
            settings.uncaught_exception_stack_size = 1;
#else
            settings.log_severity = LOGSEVERITY_DISABLE;
            //settings.pack_loading_disabled = true;
#endif

            UAssert(CefInitialize(main_args, settings, m_core.get(), nullptr),
                "Unable to initialize CEF.");

            CefBrowserSettings window_settings;

            window_settings.webgl = STATE_DISABLED;
            window_settings.accelerated_compositing = STATE_ENABLED;
            window_settings.java = STATE_DISABLED;
            window_settings.plugins = STATE_DISABLED;
            window_settings.javascript = STATE_ENABLED;
            window_settings.local_storage = STATE_DISABLED;
            window_settings.application_cache = STATE_DISABLED;
            window_settings.databases = STATE_DISABLED;
            window_settings.caret_browsing = STATE_DISABLED;
            window_settings.file_access_from_file_urls = STATE_ENABLED;
            window_settings.universal_access_from_file_urls = STATE_ENABLED;

            m_view->Initialize( path, window_settings );

            gGamepadManager->Listener( this )
                .On( GP_BUTTON_DOWN, &UISystem::onGamepadButtonDown )
                .On( GP_BUTTON_UP, &UISystem::onGamepadButtonUp )
                .On( GP_CONNECTED, &UISystem::onGamepadConnected )
                .On( GP_DISCONNECTED, &UISystem::onGamepadDisconnected );

            gKeyboardManager->Listener( this )
                .On( KM_KEY_DOWN, &UISystem::onKeyboardKeyDown )
                .On( KM_KEY_UP, &UISystem::onKeyboardKeyUp );
        }

        UISystem::~UISystem(void)
        {
            gGamepadManager->Listener( this )
                .Off( GP_BUTTON_DOWN, &UISystem::onGamepadButtonDown )
                .Off( GP_BUTTON_UP, &UISystem::onGamepadButtonUp )
                .Off( GP_CONNECTED, &UISystem::onGamepadConnected )
                .Off( GP_DISCONNECTED, &UISystem::onGamepadDisconnected );

            gKeyboardManager->Listener( this )
                .Off( KM_KEY_DOWN, &UISystem::onKeyboardKeyDown )
                .Off( KM_KEY_UP, &UISystem::onKeyboardKeyUp );

            gUISystem = nullptr;
        }

        CefRefPtr<View> UISystem::GetView(void)
        {
            return m_view;
        }

        void UISystem::Update(void)
        {
            CefDoMessageLoopWork( );

            m_view->Draw( );
        }

        void UISystem::Message(const std::string &name, const Json &data)
        {
            auto message = CefProcessMessage::Create( name );

            auto args = message->GetArgumentList( );

            args->SetSize( 1 );
            args->SetString( 0, data.dump( ) );

            m_view->GetBrowser( )->SendProcessMessage( PID_RENDERER, message );
        }

        void UISystem::onGamepadButtonDown(EVENT_HANDLER(GamepadManager))
        {
            EVENT_ATTRS(GamepadManager, GamepadButtonArgs);

            Json::object btn_event {
                { "index", static_cast<int>( args->gamepad ) },
                { "button", static_cast<int>( args->button ) },
                { "pressed", true },
                { "triggered", args->triggered }
            };

            Message( kEventGamepadDown, btn_event );
        }

        void UISystem::onGamepadButtonUp(EVENT_HANDLER(GamepadManager))
        {
            EVENT_ATTRS(GamepadManager, GamepadButtonArgs);

            Json::object btn_event {
                { "index", static_cast<int>( args->gamepad ) },
                { "button", static_cast<int>( args->button ) },
                { "pressed", false },
                { "triggered", args->triggered }
            };

            Message( kEventGamepadUp, btn_event );
        }

        void UISystem::onGamepadConnected(EVENT_HANDLER(GamepadManager))
        {
            EVENT_ATTRS(GamepadManager, GamepadConnectionArgs);

            Json::object connection_event {
                { "index", static_cast<int>( args->index ) },
                { "connected", true }
            };

            Message( kEventGamepadConnected, connection_event );
        }

        void UISystem::onGamepadDisconnected(EVENT_HANDLER(GamepadManager))
        {
            EVENT_ATTRS(GamepadManager, GamepadConnectionArgs);

            Json::object connection_event {
                { "index", static_cast<int>( args->index ) },
                { "connected", false }
            };

            Message( kEventGamepadDisconnected, connection_event );
        }

        void UISystem::onKeyboardKeyDown(EVENT_HANDLER(KeyboardManager))
        {
            EVENT_ATTRS(KeyboardManager, KeyboardKeyArgs);

            Json::object key_event {
                { "key", args->key },
                { "pressed", args->state },
                { "triggered", args->repeat == 0 }
            };

            Message( kEventKeyboardKeyDown, key_event );
        }

        void UISystem::onKeyboardKeyUp(EVENT_HANDLER(KeyboardManager))
        {
            EVENT_ATTRS(KeyboardManager, KeyboardKeyArgs);

            Json::object key_event {
                { "key", args->key },
                { "pressed", args->state }
            };

            Message( kEventKeyboardKeyUp, key_event );
        }
    }
}
