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
    ~Project(void);

    ursine::Scene::Handle GetScene(void);
    ursine::UIView::Handle GetUI(void);

    ursine::ScenePlayState GetPlayState(void) const;
    void SetPlayState(ursine::ScenePlayState state);

    void SetWorld(ursine::ecs::World *world);
private:
    ursine::Json m_worldCache;

    ursine::UIView::Handle m_ui;

    ursine::Scene::Handle m_scene;

    EditorEntityManager m_entityManager;
};