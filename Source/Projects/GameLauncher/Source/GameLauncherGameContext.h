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

    bool IsMusicMuted(void) const override;
    void MuteMusic(bool muted) override;

    bool IsAudioMuted(void) const override;
    void MuteAudio(bool muted) override;

    void ExitGame(void) override;

private:
    GameLauncher *m_launcher;
};
