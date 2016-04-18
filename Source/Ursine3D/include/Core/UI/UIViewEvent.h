/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UIViewEvent.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Window.h"

#include <cef_client.h>

namespace ursine
{
    enum UIViewEventType
    {
        UI_LOADED,
        UI_POPUP_CREATED
    };

    struct UIPopupArgs : EventArgs
    {
        std::string name;
        CefRefPtr<CefBrowser> browser;
        CefRefPtr<CefFrame> frame;
    
        const CefPopupFeatures *popupFeatures;
        CefWindowInfo *windowInfo;
        CefRefPtr<CefClient> *client;
        CefBrowserSettings *settings;

        UIPopupArgs(void) = default;
    };
}