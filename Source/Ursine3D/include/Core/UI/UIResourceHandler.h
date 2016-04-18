/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UIResourceHandler.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "UIConfig.h"
#include "UIProjectData.h"

#include <cef_scheme.h>

namespace ursine
{
    class UIResourceHandler : public CefResourceHandler
    {
    public:
        UIResourceHandler(const std::string &domain, resources::ResourceManager *manager);

    private:
        std::string m_requestPrefix;
        std::string m_mimeType;
        resources::ResourceManager *m_manager;

        uint64 m_readOffset;
        BinaryData::Handle m_resourceEntry;

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
