#pragma once

#include "RenderProcessHandler.h"

#if defined(UI_USE_SEPARATE_PROCESS)

#define UI_MAIN(argc, argv)                                       \
    auto result = ursine::UISubProcess::Initialize( argc, argv ); \
                                                                  \
    if (result >= 0)                                              \
        return result;                                            \

#else

#define UI_MAIN(argc, argv)

#endif

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