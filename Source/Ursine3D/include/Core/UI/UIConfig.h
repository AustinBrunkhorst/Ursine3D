/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Config.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    enum UIMessageCommand
    {
        UI_CMD_BROADCAST
    };

    const auto kUIDebuggingPort = 6999;
    const auto kUINativeBroadcastFunction = "NativeBroadcast";

    const auto kUIGameResourceDomain = "game";
}