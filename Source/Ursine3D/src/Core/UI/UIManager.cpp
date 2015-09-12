/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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
        , m_core( new Core )
    {
        auto *app = Application::Instance;

#ifdef PLATFORM_WINDOWS

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

#ifdef CONFIG_DEBUG

        settings.log_severity = LOGSEVERITY_WARNING;
        settings.remote_debugging_port = REMOTE_DEBUGGING_PORT;
        settings.uncaught_exception_stack_size = 1;

#else

        settings.log_severity = LOGSEVERITY_DISABLE;
        //settings.pack_loading_disabled = true;

#endif

        UAssert( CefInitialize( mainArgs, settings, m_core.get( ), nullptr ),
            "Unable to initialize CEF." );

        URSINE_TODO( "move to UIView" );
        /*CefBrowserSettings window_settings;

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

        m_view->Initialize( path, window_settings );*/

        app->Connect( APP_UPDATE, this, &UIManager::onAppUpdate );
    }

    UIManager::~UIManager(void)
    {

    }

    void UIManager::onAppUpdate(EVENT_HANDLER(Application))
    {
        CefDoMessageLoopWork( );
    }
}
