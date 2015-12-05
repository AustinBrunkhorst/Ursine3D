#include "Precompiled.h"

#include "ScoreSystem.h"
#include <WorldEvent.h>

using namespace ursine;


ENTITY_SYSTEM_DEFINITION( ScoreSystem );

ScoreSystem::ScoreSystem(ursine::ecs::World* world) : EntitySystem(world)
                                                    , m_player1Kills( 0 )
                                                    , m_player2Kills( 0 )
                                                    , m_player1Wins( 0 )
                                                    , m_player2Wins( 0 )
                                                    , m_currRound( 0 )
                                                    , m_playerCount( 0 )
                                                    , m_maxPlayerCount( 0 )
{
    return;
}
