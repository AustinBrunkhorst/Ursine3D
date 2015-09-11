/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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
    namespace ui
    {
        class BrowserProcessHandler : public CefBrowserProcessHandler
        {
            void OnContextInitialized(void) override;

            void OnBeforeChildProcessLaunch(
                CefRefPtr<CefCommandLine> command_line) override;

            void OnRenderProcessThreadCreated(
                CefRefPtr<CefListValue> extra_info) override;
        };
    }
}