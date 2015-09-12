/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BrowserMessageHandler.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <wrapper/cef_message_router.h>

using ursine::int64;

namespace ursine
{
    class BrowserMessageHandler : public CefMessageRouterBrowserSide::Handler
    {
    private:
        bool OnQuery(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            int64 query_id,
            const CefString &request,
            bool persistent,
            CefRefPtr<Callback> callback) override;

        void OnQueryCanceled(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            int64 query_id) override;
    };
}