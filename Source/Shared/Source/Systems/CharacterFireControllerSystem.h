#pragma once

#pragma once

#include <FilterSystem.h>
#include <Randomizer.h>

class CharacterFireControllerSystem : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;

public:
    CharacterFireControllerSystem( ursine::ecs::World *world );

protected:
    void Process( ursine::ecs::Entity *entity ) override;

private:
    ursine::Randomizer m_rng;

} Meta( Enable );