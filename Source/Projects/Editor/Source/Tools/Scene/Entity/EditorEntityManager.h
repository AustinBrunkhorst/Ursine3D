#pragma once

class Project;

class EditorEntityManager
{
public:
    EditorEntityManager(Project *project);
    ~EditorEntityManager(void);

private:
    Project *m_project;

    // entity events
    void onEntityAdded(EVENT_HANDLER(ursine::ecs::World));
    void onEntityRemoved(EVENT_HANDLER(ursine::ecs::World));
    void onEntityNameChanged(EVENT_HANDLER(ursine::ecs::World));

    // component events
    void onComponentAdded(EVENT_HANDLER(ursine::ecs::World));
    void onComponentRemoved(EVENT_HANDLER(ursine::ecs::World));
    void onComponentChanged(EVENT_HANDLER(ursine::ecs::World));
};