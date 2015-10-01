/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BrowserProcessHandler.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


#include "UrsinePrecompiled.h"

#include "BrowserProcessHandler.h"
#include "UIConfig.h"

namespace ursine
{
    void BrowserProcessHandler::OnContextInitialized(void)
    {

    }

    void BrowserProcessHandler::OnBeforeChildProcessLaunch(
        CefRefPtr<CefCommandLine> cmdLine)
    {
            
    }

    void BrowserProcessHandler::OnRenderProcessThreadCreated(
        CefRefPtr<CefListValue> extraInfo)
    {

    }
}