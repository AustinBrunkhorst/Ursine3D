/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BrowserMessageHandler.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


#include "UrsinePrecompiled.h"

#include "BrowserMessageHandler.h"

namespace ursine
{
    bool BrowserMessageHandler::OnQuery(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        int64 query_id,
        const CefString &request,
        bool persistent,
        CefRefPtr<Callback> callback)
    {
        return false;
    }

    void BrowserMessageHandler::OnQueryCanceled(
        CefRefPtr<CefBrowser> browser, 
        CefRefPtr<CefFrame> frame, 
        int64 query_id)
    {
            
    }
}