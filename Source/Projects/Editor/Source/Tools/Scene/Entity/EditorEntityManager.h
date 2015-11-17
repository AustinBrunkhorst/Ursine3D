#pragma once

#include <Scene.h>

class Project;

class EditorEntityManager
{
public:
    EditorEntityManager(Project *project);
    ~EditorEntityManager(void);

    void SetWorld(ursine::ecs::World::Handle world);
private:
    Project *m_project;
    ursine::ecs::World::Handle m_world;

    void clearWorld(ursine::ecs::World::Handle world);

    // entity events
    void onEntityAdded(EVENT_HANDLER(ursine::ecs::World));
    void onEntityRemoved(EVENT_HANDLER(ursine::ecs::World));
    void onEntityNameChanged(EVENT_HANDLER(ursine::ecs::World));
    void onEntityParentChanged(EVENT_HANDLER(ursine::ecs::World));

    // component events
    void onComponentAdded(EVENT_HANDLER(ursine::ecs::World));
    void onComponentRemoved(EVENT_HANDLER(ursine::ecs::World));
    void onComponentChanged(EVENT_HANDLER(ursine::ecs::World));
};