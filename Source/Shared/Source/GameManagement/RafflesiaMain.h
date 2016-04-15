#pragma once

#include <GameManager.h>

class RafflesiaMain : public ursine::GameManager
{
    NATIVE_GAME_MANAGER;

public:
    RafflesiaMain(ursine::GameContext *context, const ursine::Json &configObj);
    ~RafflesiaMain(void);

private:
} Meta(Enable);