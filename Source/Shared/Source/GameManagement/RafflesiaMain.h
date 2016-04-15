#pragma once

#include <GameManager.h>

class RafflesiaMain : public ursine::GameManager
{
    NATIVE_GAME_MANAGER;

public:
    RafflesiaMain(void);
    ~RafflesiaMain(void);

    void OnInitialize(ursine::GameContext *context, const ursine::Json &configObj) override;

private:
} Meta(Enable);