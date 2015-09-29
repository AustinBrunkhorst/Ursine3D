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

#include "BrowserProcessHandler.h"
#include "RenderProcessHandler.h"
#include "ResourceBundleHandler.h"

#include <cef_client.h>
#include "UISubProcess.h"

namespace ursine
{
    class UICore
        : public CefApp
        , public CefClient
        , public BrowserProcessHandler
        // TODO:
        , public RenderProcessHandler
        , public ResourceBundleHandler
    {
    public:
        UICore(void);
        ~UICore(void);

        static void Shutdown(void);

    private:
        void OnBeforeCommandLineProcessing(const CefString &process_type,
            CefRefPtr<CefCommandLine> command_line) override;

        CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler(void) override;
        CefRefPtr<CefResourceBundleHandler> GetResourceBundleHandler(void) override;

        // TODO:
        CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler(void) override { return this; }

        IMPLEMENT_REFCOUNTING(UICore);
    };
}