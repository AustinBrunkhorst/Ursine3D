#pragma once

#include <Scene.h>
#include <UIView.h>

class EditorEntityManager;

class Project
{
public:
    typedef std::shared_ptr<Project> Handle;

    Project(ursine::UIView::Handle ui);

    ursine::Scene &GetScene(void);

    ursine::UIView::Handle GetUI(void);

private:
    ursine::Scene m_scene;

    ursine::UIView::Handle m_ui;

    EditorEntityManager m_entityManager;
};