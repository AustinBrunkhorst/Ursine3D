/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BrowserProcessHandler.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <cef_app.h>

namespace ursine
{
    class BrowserProcessHandler : public CefBrowserProcessHandler
    {
    private:
        void OnContextInitialized(void) override;

        void OnBeforeChildProcessLaunch(
            CefRefPtr<CefCommandLine> cmdLine
        ) override;

        void OnRenderProcessThreadCreated(
            CefRefPtr<CefListValue> extraInfo
        ) override;
    };
}