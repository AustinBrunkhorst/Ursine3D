#include "Precompiled.h"

#include "RoundSystem.h"
#include <WorldEvent.h>
#include "RoundSystem.h"
#include <SystemManager.h>
#include "SpawnSystem.h"
#include <AudioManager.h>


using namespace ursine;


ENTITY_SYSTEM_DEFINITION( RoundSystem );

namespace
{
    const std::string team1Wins = "ROUND_END_BLUE";
    const std::string team2Wins = "ROUND_END_RED";
}

RoundSystem::RoundSystem(ursine::ecs::World* world) : EntitySystem(world)
                                                    , EventDispatcher(this)
                                                    , m_round(1)
                                                    , m_maxRound(5)
{

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
    m_timers.Create(TimeSpan::FromSeconds(0)).Completed(
        [=] (void)
    {
        auto *m_map = m_world->CreateEntityFromArchetype(
            WORLD_ARCHETYPE_PATH "map.uatype",
            "gameMapArchetype"
        );

		m_world->GetEntitySystem( SpawnSystem )->Listener( this )
			.On( ROUND_OVER, &RoundSystem::onRoundOver);

        RoundEventArgs e( 1 );

        Dispatch( ROUND_START, &e );
    } );
}

void RoundSystem::OnRemove()
{
    m_world->GetEntitySystem( SpawnSystem )->Listener( this )
        .Off( ROUND_OVER, &RoundSystem::onRoundOver );
}

void RoundSystem::onRoundOver(EVENT_HANDLER(ursine::ecs:::World))
{
    EVENT_ATTRS( ecs::World, RoundEventArgs );

    ++m_round;

    if (args->team == 1)
    {
        AudioManager::PlayGlobalEvent(team1Wins);

    }
    else
    {
        AudioManager::PlayGlobalEvent(team2Wins);
    }

    if (m_round > m_maxRound)
    {
        RoundEventArgs e( args->team );

        Dispatch( MATCH_OVER, &e );
    }
    else
    {
        RoundEventArgs e( m_round );

        Dispatch( ROUND_START, &e );
    }
}
