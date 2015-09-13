#include "UrsinePrecompiled.h"

#include "UISubProcess.h"

namespace ursine
{
    UISubProcess::UISubProcess(void)
    {

    }

    CefRefPtr<CefRenderProcessHandler> UISubProcess::GetRenderProcessHandler(void)
    {
        return this;
    }
}