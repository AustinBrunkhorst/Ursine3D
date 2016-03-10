/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorEntityManager.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Scene.h>

class Editor;
class Project;

class EditorEntityManager
{
public:
    EditorEntityManager(Project *project);
    ~EditorEntityManager(void);

private:
    Editor *m_editor;
    Project *m_project;
    ursine::ecs::World *m_activeWorld;

    void initWorldEvents(ursine::ecs::World *world);
    void clearWorldEvents(ursine::ecs::World *world);

    ///////////////////////////////////////////////////////////////////////////
    // Scene Events
    ///////////////////////////////////////////////////////////////////////////

    void onSceneActiveWorldChanged(EVENT_HANDLER(ursine::Scene));

    ///////////////////////////////////////////////////////////////////////////
    // World Events
    ///////////////////////////////////////////////////////////////////////////

    // entity events
    void onEntityAdded(EVENT_HANDLER(ursine::ecs::World));
    void onEntityRemoved(EVENT_HANDLER(ursine::ecs::World));
    void onEntityNameChanged(EVENT_HANDLER(ursine::ecs::World));
    void onEntityParentChanged(EVENT_HANDLER(ursine::ecs::Entity));

    // component events
    void onComponentAdded(EVENT_HANDLER(ursine::ecs::World));
    void onComponentRemoved(EVENT_HANDLER(ursine::ecs::World));
    void onComponentChanged(EVENT_HANDLER(ursine::ecs::World));
    void onComponentArrayModified(EVENT_HANDLER(ursine::ecs::World));
};