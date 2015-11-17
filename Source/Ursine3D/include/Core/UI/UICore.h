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

#include "BrowserProcessHandler.h"
#include "RenderProcessHandler.h"
#include "ResourceBundleHandler.h"

#include <cef_client.h>

namespace ursine
{
    class UICore
        : public CefApp
        , public CefClient
        , public BrowserProcessHandler
        , public RenderProcessHandler
        , public ResourceBundleHandler
    {
    public:
        static CefRefPtr<UICore> Instance;

        UICore(void);
        ~UICore(void);

        static void Shutdown(void);

    private:
        void OnBeforeCommandLineProcessing(
            const CefString &processType,
            CefRefPtr<CefCommandLine> cmdLine
        ) override;

        CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler(void) override;
        CefRefPtr<CefResourceBundleHandler> GetResourceBundleHandler(void) override;
        CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler(void) override;

        IMPLEMENT_REFCOUNTING( UICore );
    };
}