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