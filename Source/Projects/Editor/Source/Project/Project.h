/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Project.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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

    void SetWorld(ursine::ecs::World *world);

	void ClearDeletionQueue(void);

private:
    ursine::UIView::Handle m_ui;

    ursine::Scene::Handle m_scene;

    EditorEntityManager m_entityManager;
};