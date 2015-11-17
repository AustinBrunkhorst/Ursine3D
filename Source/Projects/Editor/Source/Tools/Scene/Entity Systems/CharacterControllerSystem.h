#pragma once

#include <FilterSystem.h>

class CharacterControllerSystem : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;

public:
    CharacterControllerSystem(ursine::ecs::World *world);

protected:
    void Process(ursine::ecs::Entity *entity) override;

} Meta(Enable);