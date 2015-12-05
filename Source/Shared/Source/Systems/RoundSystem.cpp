#include "Precompiled.h"

#include "RoundSystem.h"
#include <WorldEvent.h>
#include "RoundSystem.h"
#include <SystemManager.h>
#include "SpawnSystem.h"


using namespace ursine;


ENTITY_SYSTEM_DEFINITION( RoundSystem );

RoundSystem::RoundSystem(ursine::ecs::World* world) : EntitySystem(world)
                                                    ,m_round(1)
                                                    ,m_maxRound(5)
{
    RoundEventArgs e(1);

    Dispatch(ROUND_START, &e);

    auto *m_map = m_world->CreateEntityFromArchetype(
        WORLD_ARCHETYPE_PATH "map.uatype",
        "gameMapArchetype"
        );
}

int RoundSystem::GetCurrentRound(void) const
{
    return m_round;
}

int RoundSystem::GetMaxRoundCount() const
{
    return m_maxRound;
}

void RoundSystem::SetMaxRoundCount(int round)
{
    m_maxRound = round;
}

void RoundSystem::OnInitialize()
{
    m_world->Listener(this)
        .On(static_cast<ecs::WorldEventType>(ROUND_OVER), &RoundSystem::onRoundOver);
}

void RoundSystem::OnRemove()
{
    m_world->Listener(this)
        .Off(static_cast<ecs::WorldEventType>(ROUND_OVER), &RoundSystem::onRoundOver);
}

void RoundSystem::onRoundOver(EVENT_HANDLER(ursine::ecs:::World))
{
    EVENT_ATTRS(ecs::World, RoundEventArgs);

    ++m_round;

    if (m_round > m_maxRound)
    {
        RoundEventArgs e(args->team);

        Dispatch(MATCH_OVER, &e);
    }
    else
    {
        RoundEventArgs e(m_round);

        Dispatch(ROUND_START, &e);
    }
}
