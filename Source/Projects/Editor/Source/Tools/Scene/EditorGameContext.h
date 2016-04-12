#pragma once

#include <GameContext.h>

class Project;

class EditorGameContext : public ursine::GameContext
{
public:
    EditorGameContext(Project *project);

    bool GetWindowFullScreen(void) const override;
    void SetWindowFullScreen(bool fullScreen) override;

    bool IsMusicMuted(void) const override;
    void MuteMusic(bool muted) override;

    bool IsAudioMuted(void) const override;
    void MuteAudio(bool muted) override;

    void ExitGame(void) override;

private:
    Project *m_project;
};