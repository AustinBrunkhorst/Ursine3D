#include "Precompiled.h"

#include "Project.h"
#include "EntityManager.h"

Project::Project(ursine::UIView::Handle ui)
    : m_ui( ui )
    , m_entityManager( Handle( this ) )
{
    
}

ursine::Scene &Project::GetScene(void)
{
    return m_scene;
}

ursine::UIView::Handle Project::GetUI(void)
{
    return m_ui;
}