#pragma once

#include "RenderProcessHandler.h"

#define UI_MAIN(argc, argv)                                       \
    auto result = ursine::UISubProcess::Initialize( argc, argv ); \
                                                                  \
    if (result >= 0)                                              \
        return result;                                            \

namespace ursine
{
    class UISubProcess 
        : public CefApp
        , public RenderProcessHandler
    {
    public:
        static int Initialize(int argc, char **argv);

        UISubProcess(void);

    private:
        CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler(void) override;

        IMPLEMENT_REFCOUNTING(UISubProcess);
    };
}