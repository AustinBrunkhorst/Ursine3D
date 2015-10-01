#include "UrsinePrecompiled.h"

#include "UISubProcess.h"

namespace ursine
{
    int UISubProcess::Initialize(int argc, char **argv)
    {
    #if defined(PLATFORM_WINDOWS)

            CefMainArgs args( GetModuleHandle( nullptr ) );

    #else

            CefMainArgs args( argc, argv );

    #endif

        CefRefPtr<UISubProcess> app( new UISubProcess );

        return CefExecuteProcess( args, app, nullptr );
    }

    UISubProcess::UISubProcess(void)
    {

    }

    CefRefPtr<CefRenderProcessHandler> UISubProcess::GetRenderProcessHandler(void)
    {
        return this;
    }
}