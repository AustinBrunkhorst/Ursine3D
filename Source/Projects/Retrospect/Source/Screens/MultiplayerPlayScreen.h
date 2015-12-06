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
} Meta(Enable, WhiteListMethods);