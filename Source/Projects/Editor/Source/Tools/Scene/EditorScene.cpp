/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorScene.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "EditorScene.h"

EditorScene::EditorScene(void)
    : m_playState( PS_EDITOR ) { }

ScenePlayState EditorScene::GetPlayState(void) const
{
    return m_playState;
}

void EditorScene::SetPlayState(ScenePlayState state)
{
    if (state == m_playState)
        return;

    ScenePlayStateChangedArgs e( m_playState, state );

    m_playState = state;

    Dispatch( EDITOR_SCENE_PLAYSTATE_CHANGED, &e );
}

bool EditorScene::IsPaused(void) const
{
    return m_playState != PS_PLAYING || Scene::IsPaused( );
}

void EditorScene::Step(void) const
{
    if (m_activeWorld)
        m_activeWorld->Update( );

    SceneFrameSteppedArgs e( ursine::Application::Instance->GetDeltaTime( ) );

    Dispatch( EDITOR_SCENE_FRAME_STEPPED, &e );
}

void EditorScene::Update(ursine::DeltaTime dt) const
{
    if (!m_activeWorld)
        return;

    switch (m_playState)
    {
    case PS_PAUSED:
    case PS_EDITOR:
        m_activeWorld->EditorUpdate( );
        break;

    case PS_PLAYING:
        Scene::Update( dt );
        break;
    }
}

void EditorScene::Render(void) const
{
    if (!m_activeWorld)
        return;

    switch (m_playState)
    {
    case PS_PAUSED:
    case PS_EDITOR:
        m_activeWorld->EditorRender( );
        break;

    case PS_PLAYING:
        Scene::Render( );
        break;
    }
}
