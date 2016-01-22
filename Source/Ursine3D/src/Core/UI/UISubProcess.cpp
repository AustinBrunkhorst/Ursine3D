/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UISubProcess.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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