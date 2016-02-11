/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Core.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UICore.h"

namespace ursine
{
    CefRefPtr<UICore> UICore::Instance = nullptr;

    UICore::UICore(void)
    {
        atexit( Shutdown ); 
    }

    UICore::~UICore(void)
    {
            
    }

    void UICore::OnBeforeCommandLineProcessing(
        const CefString &processType,
        CefRefPtr<CefCommandLine> cmdLine
    )
    {
        cmdLine->AppendSwitch( "disable-extensions" );

        // fixes v8 errors on startup
        cmdLine->AppendSwitch( "no-proxy-server" );
        cmdLine->AppendSwitch( "winhttp-proxy-resolver" );

        cmdLine->AppendSwitch( "enable-begin-frame-scheduling" );
        cmdLine->AppendSwitch( "disable-gpu" );
        cmdLine->AppendSwitch( "disable-gpu-compositing" );

        //command_line->AppendSwitch("off-screen-rendering-enabled");
        //command_line->AppendSwitch("in-process-gpu");
        //command_line->AppendSwitchWithValue("use-gl", "desktop");
    }

    CefRefPtr<CefBrowserProcessHandler> UICore::GetBrowserProcessHandler(void)
    {
        return this;
    }

    CefRefPtr<CefResourceBundleHandler> UICore::GetResourceBundleHandler(void)
    {
        return this;
    }

    CefRefPtr<CefRenderProcessHandler> UICore::GetRenderProcessHandler(void)
    {
        return this;
    }

    void UICore::Shutdown(void)
    {
        URSINE_TODO( "Find CEF reference counting leak" );
    #if !defined(CONFIG_DEBUG)

        CefShutdown( );

    #endif
    }
}