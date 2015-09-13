/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

#include "Core.h"

namespace ursine
{
    Core::Core(void)
    {
        atexit( Shutdown );
    }

    Core::~Core(void)
    {
            
    }

    void Core::OnBeforeCommandLineProcessing(const CefString &process_type,
        CefRefPtr<CefCommandLine> command_line)
    {
        // fixes v8 errors on startup
        command_line->AppendSwitch( "no-proxy-server" );
        command_line->AppendSwitch( "winhttp-proxy-resolver" );

        command_line->AppendSwitch( "enable-begin-frame-scheduling" );
        command_line->AppendSwitch( "disable-gpu" );
        command_line->AppendSwitch( "disable-gpu-compositing" );

        //command_line->AppendSwitch("off-screen-rendering-enabled");
        //command_line->AppendSwitch("in-process-gpu");
        //command_line->AppendSwitchWithValue("use-gl", "desktop");
    }

    CefRefPtr<CefBrowserProcessHandler> Core::GetBrowserProcessHandler(void)
    {
        return this;
    }

    CefRefPtr<CefResourceBundleHandler> Core::GetResourceBundleHandler(void)
    {
        return this;
    }

    bool Core::GetLocalizedString(int nessageID, CefString &string)
    {
        // dot to shut up the errors
        string = ".";

        return true;
    }

    void Core::Shutdown(void)
    {
        CefShutdown( );
    }
}