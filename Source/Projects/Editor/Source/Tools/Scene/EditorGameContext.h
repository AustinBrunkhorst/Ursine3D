#pragma once

#include <GameContext.h>

class Project;

class EditorGameContext : public ursine::GameContext
{
public:
    EditorGameContext(Project *project);

    bool GetWindowFullScreen(void) const override;
    void SetWindowFullScreen(bool fullScreen) override;

    void SetVolume(float volume, const std::string &outputType) const override;

    void ExitGame(void) override;

private:
    Project *m_project;
};