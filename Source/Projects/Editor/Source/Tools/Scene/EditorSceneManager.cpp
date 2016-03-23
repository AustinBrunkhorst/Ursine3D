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
        }
    }
}

EditorSceneManager::EditorSceneManager(Project *project)
    : m_editor( GetCoreSystem( Editor ) )
    , m_project( project )
{
     m_project->GetScene( ).Listener( this )
        .On( SCENE_WORLD_CHANGED, &EditorSceneManager::onSceneActiveWorldChanged )
        .On( SCENE_PLAYSTATE_CHANGED, &EditorSceneManager::onScenePlayStateChanged );
}

EditorSceneManager::~EditorSceneManager(void)
{
    m_project->GetScene( ).Listener( this )
        .Off( SCENE_WORLD_CHANGED, &EditorSceneManager::onSceneActiveWorldChanged )
        .Off( SCENE_PLAYSTATE_CHANGED, &EditorSceneManager::onScenePlayStateChanged );
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