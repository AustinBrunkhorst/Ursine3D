#include "Precompiled.h"

#include "Project.h"
#include "EditorEntityManager.h"

using namespace ursine;

Project::Project(UIView::Handle ui)
    : m_ui( ui )
    , m_scene( std::make_shared<Scene>( ) )
    , m_entityManager( this )
{
    m_entityManager.SetWorld( m_scene->GetWorld( ) );
}

Scene::Handle Project::GetScene(void)
{
    return m_scene;
}

UIView::Handle Project::GetUI(void)
{
    return m_ui;
}

void Project::SetWorld(ecs::World::Handle world)
{
    m_scene->SetWorld( world );
    m_entityManager.SetWorld( world );
}