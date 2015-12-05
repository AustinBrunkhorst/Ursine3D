#include "Precompiled.h"

#include "ScoreSystem.h"
#include <WorldEvent.h>

using namespace ursine;


ENTITY_SYSTEM_DEFINITION( ScoreSystem );

ScoreSystem::ScoreSystem(ursine::ecs::World* world) : EntitySystem(world)
                                                    , m_playerCount(0)
                                                    , m_maxPlayerCount(0)
{
    return;
}

void ScoreSystem::OnInitialize()
{
    return;
}

void ScoreSystem::OnRemove()
{
    return;
}