/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Core.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "EventDispatcher.h"
#include "CoreEvent.h"

#include "BrowserProcessHandler.h"
#include "RenderProcessHandler.h"
#include "ResourceBundleHandler.h"

#include <cef_client.h>

namespace ursine
{
    namespace ui
    {
        class Core
            : public CefApp
            , public CefClient
            , public BrowserProcessHandler
            , public RenderProcessHandler
            , public ResourceBundleHandler
        {
            void OnBeforeCommandLineProcessing(const CefString &process_type,
                CefRefPtr<CefCommandLine> command_line) override;

            CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler(void) override;
            CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler(void) override;
            CefRefPtr<CefResourceBundleHandler> GetResourceBundleHandler(void) override;

            bool GetLocalizedString(int message_id, CefString &string) override;

        public:
            Core(void);
            ~Core(void);

            static void Shutdown(void);

            IMPLEMENT_REFCOUNTING(Core);
        };
    }
}