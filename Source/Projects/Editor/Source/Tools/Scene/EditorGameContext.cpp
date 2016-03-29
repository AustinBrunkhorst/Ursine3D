#include "Precompiled.h"

#include "EditorGameContext.h"

#include "Project.h"

EditorGameContext::EditorGameContext(Project *project)
    : GameContext( this )
    , m_project( project ) { }

void EditorGameContext::ExitGame(void)
{
    m_project->GetScene( ).SetPlayState( ursine::PS_EDITOR );
}
