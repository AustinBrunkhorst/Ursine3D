/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorGameContext.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "EditorGameContext.h"

#include "Project.h"

#include "AudioSystem.h"

EditorGameContext::EditorGameContext(Project *project)
    : GameContext( &project->GetScene( ), this )
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

float EditorGameContext::GetVolume(const std::string &outputType) const
{
    auto search = m_volumeCache.find( outputType );

    return search == m_volumeCache.end( ) ? 1.0f : search->second;
}

void EditorGameContext::SetVolume(float volume, const std::string &outputType)
{
    ursine::ecs::AudioSystem::SetRealTimeParameter( outputType, volume * 100.0f );

    m_volumeCache[ outputType ] = volume;
}

void EditorGameContext::ExitGame(void)
{
    m_project->GetScene( ).SetPlayState( PS_EDITOR );
}
