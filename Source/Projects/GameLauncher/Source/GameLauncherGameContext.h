#pragma once

#include <GameContext.h>

const ursine::Vec2 kDefaultWindowDimensions { 1280, 720 };

class GameLauncher;

class GameLauncherGameContext : public ursine::GameContext
{
public:
    GameLauncherGameContext(ursine::Scene *scene);

    bool GetWindowFullScreen(void) const override;
    void SetWindowFullScreen(bool fullScreen) override;

    float GetVolume(const std::string &outputType) const override;
    void SetVolume(float volume, const std::string &outputType) override;

    void ExitGame(void) override;

private:
    std::unordered_map<std::string, float> m_volumeCache;

    GameLauncher *m_launcher;
};
