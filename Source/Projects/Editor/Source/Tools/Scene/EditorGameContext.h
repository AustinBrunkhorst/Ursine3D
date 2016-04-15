#pragma once

#include <GameContext.h>

class Project;

class EditorGameContext : public ursine::GameContext
{
public:
    EditorGameContext(Project *project);

    bool GetWindowFullScreen(void) const override;
    void SetWindowFullScreen(bool fullScreen) override;

    float GetVolume(const std::string &outputType) const override;
    void SetVolume(float volume, const std::string &outputType) override;

    void ExitGame(void) override;

private:
    std::unordered_map<std::string, float> m_volumeCache;

    Project *m_project;
};