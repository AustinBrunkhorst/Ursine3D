#pragma once

#include <GameManager.h>

class RafflesiaMain : public ursine::GameManager
{
    NATIVE_GAME_MANAGER;

public:
    RafflesiaMain(void);
    ~RafflesiaMain(void);

private:
    void OnInitialize(ursine::GameContext *context, const ursine::Json &configObj) override;

    void onWindowFocusChanged(EVENT_HANDLER(ursine::GameContext));

    void onMainMenuEntered(EVENT_HANDLER(ursine::UIScreenManager));
    void onGameplayStarted(EVENT_HANDLER(ursine::UIScreenManager));
    void onGameplayPaused(EVENT_HANDLER(ursine::UIScreenManager));
    void onGameplayResumed(EVENT_HANDLER(ursine::UIScreenManager));
    void onGameplayExited(EVENT_HANDLER(ursine::UIScreenManager));
    void onGameplayRestarted(EVENT_HANDLER(ursine::UIScreenManager));
} Meta(Enable);