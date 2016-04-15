#pragma once

#include <GameContext.h>

const ursine::Vec2 kDefaultWindowDimensions { 1280, 720 };

class GameLauncher;

class GameLauncherGameContext : public ursine::GameContext
{
public:
    GameLauncherGameContext(void);

    bool GetWindowFullScreen(void) const override;
    void SetWindowFullScreen(bool fullScreen) override;

    void SetVolume(float volume, const std::string &outputType) const;

    void ExitGame(void) override;

private:
    GameLauncher *m_launcher;
};
