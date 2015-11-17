#pragma once

#include <EntitySystem.h>

class EditorIconSystem : public ursine::ecs::EntitySystem
{
    ENTITY_SYSTEM;
public:
    EditorIconSystem(ursine::ecs::World *world);

private:
    void OnInitialize(void) override;
    void OnRemove(void) override;

    void onIconAdd(EVENT_HANDLER(ursine::ecs::World));
} Meta(Enable);