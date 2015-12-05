#include "Precompiled.h"

#include "ScoreSystem.h"

#include <SystemManager.h>
#include <WorldEvent.h>

using namespace ursine;


ENTITY_SYSTEM_DEFINITION( ScoreSystem );

ScoreSystem::ScoreSystem(ursine::ecs::World* world) : EntitySystem(world)
                                                    , m_player1Kills( 0 )
                                                    , m_player2Kills( 0 )
                                                    , m_player1Wins( 0 )
                                                    , m_player2Wins( 0 )
                                                    , m_currRound( 1 )
{
    return;
}

int ScoreSystem::GetKills(int player) const
{
    // just using this since if someone tries
    // for bad players score I don't mind wrong val
    return player == 1 ? m_player1Kills : m_player2Kills;
}

void ScoreSystem::AddKill(int player)
{
    if (player == 1)
    {
        ++m_player1Kills;
    }
    else if (player == 2)
    {
        ++m_player2Kills;
    }
}

int ScoreSystem::GetCurrentRound() const
{
    return m_currRound;
}

void ScoreSystem::OnInitialize(void)
{
    m_world->GetEntitySystem(RoundSystem)->Listener(this)
        .On(ROUND_OVER, &ScoreSystem::onRoundOver);
}

void ScoreSystem::OnRemove()
{
    m_world->GetEntitySystem(RoundSystem)->Listener(this)
        .Off(ROUND_OVER, &ScoreSystem::onRoundOver);
}

void ScoreSystem::onRoundOver(EVENT_HANDLER(RoundSystem))
{
    EVENT_ATTRS(RoundSystem, RoundSystem::RoundEventArgs);

    // this means team1 lost
    if (args->team == 1)
    {
        ++m_player2Wins;
    }
    // this means team2 lost
    else if (args->team == 2)
    {
        ++m_player1Wins;
    }

    ++m_currRound;
}