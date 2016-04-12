#include "Precompiled.h"

#include "EditorGameContext.h"

#include "Project.h"

EditorGameContext::EditorGameContext(Project *project)
    : GameContext( this )
    , m_project( project ) { }

bool EditorGameContext::GetWindowFullScreen(void) const
{
    // always false
    return false;
}

void EditorGameContext::SetWindowFullScreen(bool fullScreen)
{
    // do nothing
}

bool EditorGameContext::IsMusicMuted(void) const
{
    // always false
    return false;
}

void EditorGameContext::MuteMusic(bool muted)
{
    
}

bool EditorGameContext::IsAudioMuted(void) const
{
    // always false
    return false;
}

void EditorGameContext::MuteAudio(bool muted)
{
    
}

void EditorGameContext::ExitGame(void)
{
    m_project->GetScene( ).SetPlayState( PS_EDITOR );
}
