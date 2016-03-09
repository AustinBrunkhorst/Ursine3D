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