/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MultiplayerPlayScreen.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <ScreenManager.h>
#include <GameplayScreen.h>

class MultiplayerPlayScreen : public ursine::GameplayScreen
{
    NATIVE_SCREEN

public:
    Meta(Enable)
    MultiplayerPlayScreen(ursine::ScreenManager *manager);
    virtual ~MultiplayerPlayScreen(void);

private:
    void onMainWindowFocusChanged(EVENT_HANDLER(ursine::Window));

    void loadSystems(void);
} Meta(Enable, WhiteListMethods);