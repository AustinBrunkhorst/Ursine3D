/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UIManager.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UIManager.h"

namespace ursine
{
    CORE_SYSTEM_DEFINITION( UIManager );

    UIManager::UIManager(void)
        : EventDispatcher( this )
    {
        auto *app = Application::Instance;

    #if defined(PLATFORM_WINDOWS)

        CefMainArgs mainArgs( GetModuleHandle( nullptr ) );

    #else

        CefMainArgs mainArgs( app->GetArgC( ), app->GetArgV( ) );

    #endif
        CefSettings settings;

        settings.no_sandbox = true;
        settings.ignore_certificate_errors = true;
        settings.command_line_args_disabled = true;

        settings.single_process = true;
        settings.multi_threaded_message_loop = false;

    #if defined(CONFIG_DEBUG)

        settings.log_severity = LOGSEVERITY_WARNING;
        settings.uncaught_exception_stack_size = 1;

    #else

        settings.log_severity = LOGSEVERITY_DISABLE;

    #endif

    #if defined(URSINE_WITH_EDITOR)

        settings.remote_debugging_port = kUIDebuggingPort;

    #endif

        settings.background_color = CefColorSetARGB( 0, 0, 0, 0 );

        UICore::Instance = new UICore( );

        UAssert( CefInitialize( mainArgs, settings, UICore::Instance, nullptr ),
            "Unable to initialize CEF." 
        );

        CefEnableHighDPISupport( );

        app->Connect( APP_UPDATE, this, &UIManager::onAppUpdate );
    }

    UIManager::~UIManager(void)
    {
        Application::Instance->Disconnect( APP_UPDATE, this, &UIManager::onAppUpdate );
    }

    UIView::Handle UIManager::CreateView(Window::Handle window, const std::string &url) const
    {
        CefBrowserSettings settings;

        settings.windowless_frame_rate = 144;
        settings.webgl = STATE_DISABLED;
        settings.plugins = STATE_DISABLED;
        settings.javascript = STATE_ENABLED;
        settings.javascript_access_clipboard = STATE_ENABLED;
        settings.javascript_dom_paste = STATE_ENABLED;
        settings.local_storage = STATE_DISABLED;
        settings.databases = STATE_DISABLED;
        settings.caret_browsing = STATE_DISABLED;
        settings.file_access_from_file_urls = STATE_ENABLED;
        settings.universal_access_from_file_urls = STATE_ENABLED;

        return new UIView( window, settings, url );
    }

    void UIManager::OnRegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> registrar)
    {
        registrar->AddCustomScheme( kUIGameResourceScheme, true, false, false );
    }

    void UIManager::onAppUpdate(EVENT_HANDLER(Application))
    {
        CefDoMessageLoopWork( );
    }
}
