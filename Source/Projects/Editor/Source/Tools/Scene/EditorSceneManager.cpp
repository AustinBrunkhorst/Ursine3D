/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorSceneManager.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "EditorSceneManager.h"

#include "Editor.h"
#include "Project.h"

using namespace ursine;

namespace
{
    namespace channel
    {
        const auto SceneManager = "SceneManager";
    }

    namespace events
    {
        namespace scene
        {
            const auto WorldChanged = "WorldChanged";
            const auto PlayStateChanged = "PlayStateChanged";
            const auto FrameStepped = "FrameStepped";
        }
    }
}

EditorSceneManager::EditorSceneManager(Project *project)
    : m_editor( GetCoreSystem( Editor ) )
    , m_project( project )
{
     m_project->GetScene( ).Listener( this )
        .On( SCENE_WORLD_CHANGED, &EditorSceneManager::onSceneActiveWorldChanged )
        .On( EDITOR_SCENE_PLAYSTATE_CHANGED, &EditorSceneManager::onScenePlayStateChanged )
        .On( EDITOR_SCENE_FRAME_STEPPED, &EditorSceneManager::onSceneFrameStepped );
}

EditorSceneManager::~EditorSceneManager(void)
{
    m_project->GetScene( ).Listener( this )
        .Off( SCENE_WORLD_CHANGED, &EditorSceneManager::onSceneActiveWorldChanged )
        .Off( EDITOR_SCENE_PLAYSTATE_CHANGED, &EditorSceneManager::onScenePlayStateChanged )
        .Off( EDITOR_SCENE_FRAME_STEPPED, &EditorSceneManager::onSceneFrameStepped );
}

void EditorSceneManager::onSceneActiveWorldChanged(EVENT_HANDLER(Scene))
{
    EVENT_ATTRS(Scene, SceneWorldChangedArgs);

    m_editor->GetMainWindow( ).GetUI( )->Message(
        UI_CMD_BROADCAST,
        channel::SceneManager,
        events::scene::WorldChanged,
        { }
    );
}

void EditorSceneManager::onScenePlayStateChanged(EVENT_HANDLER(Scene))
{
    EVENT_ATTRS(Scene, ScenePlayStateChangedArgs);

    auto data = Json::object {
        { "state", static_cast<int>( m_project->GetScene( ).GetPlayState( ) ) }
    };

    m_editor->GetMainWindow( ).GetUI( )->Message(
        UI_CMD_BROADCAST,
        channel::SceneManager,
        events::scene::PlayStateChanged,
        data
    );
}

void EditorSceneManager::onSceneFrameStepped(EVENT_HANDLER(Scene))
{
    EVENT_ATTRS(Scene, SceneFrameSteppedArgs);

    auto data = Json::object {
        { "dt", static_cast<int>( args->dt * TimeSpan::MillisPerSecond ) }
    };

    m_editor->GetMainWindow( ).GetUI( )->Message(
        UI_CMD_BROADCAST,
        channel::SceneManager,
        events::scene::FrameStepped,
        data
    );
}