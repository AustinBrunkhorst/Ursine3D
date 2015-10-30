#pragma once

#include <Scene.h>
#include <UIView.h>

class EditorEntityManager;

class Project
{
public:
    typedef std::shared_ptr<Project> Handle;

    Project(ursine::UIView::Handle ui);

    ursine::Scene::Handle GetScene(void);
    ursine::UIView::Handle GetUI(void);

    void SetWorld(ursine::ecs::World::Handle world);

private:
    ursine::UIView::Handle m_ui;

    ursine::Scene::Handle m_scene;

    EditorEntityManager m_entityManager;
};