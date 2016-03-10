#pragma once

#include "UIConfig.h"
#include "UIResourceData.h"

#include <cef_scheme.h>

namespace ursine
{
    class UIResourceHandler : public CefResourceHandler
    {
    public:
        UIResourceHandler(const std::string &scheme, resources::ResourceManager *resources);

    private:
        std::string m_requestPrefix;
        resources::ResourceManager *m_resources;
        resources::UIResourceData *m_uiResource;
        fs::path m_subResourcePath;

        bool ProcessRequest(
            CefRefPtr<CefRequest> request,
            CefRefPtr<CefCallback> callback
        ) override;

        void GetResponseHeaders(
            CefRefPtr<CefResponse> response,
            int64 &responseLength,
            CefString &redirectUrl
        ) override;

        bool ReadResponse(
            void *dataOut,
            int bytesToRead,
            int &bytesRead,
            CefRefPtr<CefCallback> callback
        ) override;

        void Cancel(void) override;

        void loadResource(
            const GUID &guid,
            const fs::path &path,
            CefRefPtr<CefCallback> callback
        );

        IMPLEMENT_REFCOUNTING(UIResourceHandler);
    };
}
