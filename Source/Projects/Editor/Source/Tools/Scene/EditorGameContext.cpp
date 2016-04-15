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

void EditorGameContext::SetVolume(float volume, const std::string &outputType) const
{
    auto world = m_project->GetScene( ).GetActiveWorld( );

    if (world)
    {
        ursine::ecs::VolumeChangeArgs args( volume, outputType );

        world->Dispatch( ursine::ecs::WORLD_VOLUME_CHANGE, &args );
    }
}

void EditorGameContext::ExitGame(void)
{
    m_project->GetScene( ).SetPlayState( PS_EDITOR );
}
