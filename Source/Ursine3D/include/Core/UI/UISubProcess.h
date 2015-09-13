#pragma once

#include "RenderProcessHandler.h"

namespace ursine
{
    class UISubProcess 
        : public CefApp
        , public RenderProcessHandler
    {
    public:
        UISubProcess(void);

    private:
        CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler(void) override;

        IMPLEMENT_REFCOUNTING(UISubProcess);
    };
}