/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Project.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "Project.h"
#include "EditorEntityManager.h"

#include <WorldSerializer.h>
#include <WorldConfigComponent.h>

using namespace ursine;

Project::Project(UIView::Handle ui)
    : m_ui( ui )
    , m_scene( std::make_shared<Scene>( ) )
    , m_entityManager( this )
{
    m_entityManager.SetWorld( m_scene->GetWorld( ) );
}

Project::~Project(void)
{

}

Scene::Handle Project::GetScene(void)
{
    return m_scene;
}

UIView::Handle Project::GetUI(void)
{
    return m_ui;
}

ScenePlayState Project::GetPlayState(void) const
{
    return m_scene->GetPlayState( );
}

void Project::SetPlayState(ScenePlayState state)
{
    auto lastState = m_scene->GetPlayState( );

    m_scene->SetPlayState( state );

    if (lastState == PS_EDITOR && (state == PS_PLAYING || state == PS_PAUSED))
    {
        ecs::WorldSerializer serializer;

        auto *oldWorld = m_scene->GetWorld( );

        m_worldCache = serializer.Serialize( oldWorld );

        auto *playWorld = serializer.Deserialize( m_worldCache );

        playWorld->GetSettings( )->GetComponent<ecs::WorldConfig>( )->SetInEditorMode( false );

        SetWorld( playWorld );

        m_scene->LoadConfiguredSystems( );
    }
    else if((lastState == PS_PLAYING || lastState == PS_PAUSED) && state == PS_EDITOR)
    {
        auto *world = m_scene->GetWorld( );

        ecs::WorldSerializer serializer;

        auto *cachedWorld = serializer.Deserialize( m_worldCache );

        SetWorld( cachedWorld );

        cachedWorld->GetSettings( )->GetComponent<ecs::WorldConfig>( )->SetInEditorMode( true );

        delete world;
    } 
    else
    {
        m_scene->GetWorld( )->GetSettings( )->GetComponent<ecs::WorldConfig>( )->SetInEditorMode( 
            state == PS_PAUSED 
        );
    }
}

void Project::SetWorld(ecs::World *world)
{
	m_entityManager.SetWorld( world );
    m_scene->SetWorld( world );

	m_entityManager.RelayUIResetWorld( );
}