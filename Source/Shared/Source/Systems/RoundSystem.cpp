/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RoundSystem.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "RoundSystem.h"
#include <WorldEvent.h>
#include "RoundSystem.h"
#include <SystemManager.h>
#include "SpawnSystem.h"
#include <AudioManager.h>
#include <Screen.h>
#include <ScreenManager.h>

#include "HealthComponent.h"
#include "CommandQueueComponent.h"

using namespace ursine;

ENTITY_SYSTEM_DEFINITION( RoundSystem );

namespace
{
    const std::string team1Wins = "ROUND_END_BLUE";
    const std::string team2Wins = "ROUND_END_RED";

    const auto kUIPlayerDied = "PlayerDied";
    const auto kUIPlayerDamageTaken = "PlayerDamageTaken";
    const auto kUIRoundReset = "RoundReset";

    const auto kRoundOverlayScreenName = "RoundOverlayScreen";
    const auto kMainMenuScreenName = "MainMenuScreen";
}

RoundSystem::RoundSystem(ecs::World *world)
    : EntitySystem( world )
    , EventDispatcher( this )
    , m_round( 1 )
    , m_maxRound( 5 )
{

}

int RoundSystem::GetCurrentRound(void) const
{
    return m_round;
}

int RoundSystem::GetMaxRoundCount(void) const
{
    return m_maxRound;
}

void RoundSystem::SetMaxRoundCount(int round)
{
    m_maxRound = round;
}

void RoundSystem::SendPlayerDiedMessage(ecs::Entity *died)
{
    RoundEventArgs e( died );

    Dispatch( PLAYER_DIED, &e );

    auto *teamComponent = died->GetComponent<TeamComponent>( );

    m_world->GetOwner( )->MessageUI( kUIPlayerDied, Json::object {
        { "player", teamComponent->GetTeamNumber( ) },
    } );
}

void RoundSystem::SendPlayerDamageTaken(ecs::Entity *damaged)
{
    auto *commandQueue = damaged->GetComponent<CommandQueue>( );

    // not an input player
    if (!(commandQueue && commandQueue->IsRecording( )))
        return;

    auto *healthComponent = damaged->GetComponent<Health>( );

    auto health = healthComponent->GetHealth( );
    auto maxHealth = healthComponent->GetMaxHealth( );

    auto percent = math::IsZero( maxHealth ) ? 0.0f : health / maxHealth;

    auto *teamComponent = damaged->GetComponent<TeamComponent>( );

    m_world->GetOwner( )->MessageUI( kUIPlayerDamageTaken, Json::object {
        { "player", teamComponent->GetTeamNumber( ) },
        { "percentage", percent }
    } );
}

void RoundSystem::StartNewRound(int winner)
{
    RoundEventArgs e( winner );

    Dispatch( ROUND_OVER, &e );

    if (m_round > m_maxRound)
        return;

    auto *screen = m_world->GetOwner( );

    screen->MessageUI( kUIRoundReset, Json::object { } );

    screen->GetManager( )->AddOverlay( kRoundOverlayScreenName, Json::object {
        { "title", "Round " + std::to_string( m_round ) }
    } );
}

void RoundSystem::OnInitialize(void)
{
    m_timers.Create( TimeSpan::FromSeconds( 0 ) ).Completed(
        [=] (void)
    {
        m_map = m_world->CreateEntityFromArchetype(
            WORLD_ARCHETYPE_PATH "map.uatype",
            "gameMapArchetype"
        );

        m_world->GetEntitySystem( RoundSystem )->Listener( this )
            .On( ROUND_OVER, &RoundSystem::onRoundOver );

        RoundEventArgs e( 1 );

        Dispatch( ROUND_START, &e );
    } );
}

void RoundSystem::OnRemove(void)
{
    m_map->Delete( );

    m_world->GetEntitySystem( RoundSystem)->Listener( this )
        .Off( ROUND_OVER, &RoundSystem::onRoundOver );
}

void RoundSystem::matchOver(int winner)
{
    RoundEventArgs e( winner );

    Dispatch( MATCH_OVER, &e );

    auto *screen = m_world->GetOwner( );

    screen->GetManager( )->AddOverlay( kRoundOverlayScreenName, Json::object {
        { "title", "Player " + std::to_string( winner ) + " Wins" },
        { "targetScreen", kMainMenuScreenName }
    } );
}

void RoundSystem::onRoundOver(EVENT_HANDLER(ecs:::World))
{
    EVENT_ATTRS( ecs::World, RoundEventArgs );

    ++m_round;

    if (args->team == 1)
    {
        AudioManager::PlayGlobalEvent( team1Wins );
    }
    else
    {
        AudioManager::PlayGlobalEvent( team2Wins );
    }

    if (m_round > m_maxRound)
    {
        matchOver( args->team );
    }
    else
    {
        RoundEventArgs e( m_round );

        Dispatch( ROUND_START, &e );
    }
}
