/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorUIResourceHandlerFactory.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <cef_scheme.h>

class EditorUIResourceHandlerFactory : public CefSchemeHandlerFactory
{
    CefRefPtr<CefResourceHandler> Create(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        const CefString &schemeName,
        CefRefPtr<CefRequest> request
    ) override;

    IMPLEMENT_REFCOUNTING(EditorUIResourceHandlerFactory);
};