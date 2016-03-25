#pragma once

#include <GameContext.h>

class Project;

class EditorGameContext : public ursine::GameContext
{
public:
    EditorGameContext(Project *project);

    void ExitGame(void) override;

private:
    Project *m_project;
};