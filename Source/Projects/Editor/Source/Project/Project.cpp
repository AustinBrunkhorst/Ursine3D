#include "Precompiled.h"

#include "Project.h"
#include "EditorEntityManager.h"

using namespace ursine;

Project::Project(UIView::Handle ui)
    : m_ui( ui )
    , m_scene( std::make_shared<Scene>( ) )
    , m_entityManager( this )
{
    
}

Scene::Handle Project::GetScene(void)
{
    return m_scene;
}

void Project::SetScene(Scene::Handle scene)
{
    scene = m_scene;
}

UIView::Handle Project::GetUI(void)
{
    return m_ui;
}