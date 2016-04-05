/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossPhse3VineLoopState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossPhase3VineLoopState.h"

#include "VineSpawnerComponent.h"

#include <World.h>

using namespace ursine;
using namespace ecs;

BossPhase3VineLoopState::BossPhase3VineLoopState(void)
    : BossAIState( "Phase 3 Vine Loop" ) { }

void BossPhase3VineLoopState::Enter(BossAIStateMachine *machine)
{
    m_boss = machine->GetBoss( );

    m_boss->SetVineHealthThresholdCallback( std::bind( 
        &BossPhase3VineLoopState::onThresholdHit, this, std::placeholders::_1 
    ) );
}

void BossPhase3VineLoopState::onThresholdHit(VineAI *vine)
{
    // set boss's new spawn position to that vine's current home location
    m_boss->SetHomeLocation( vine->GetHomeLocation( ) );

    // set the vine's new home location
    auto world = m_boss->GetOwner( )->GetWorld( );

    auto spawners = world->GetEntitiesFromFilter( Filter( ).All<VineSpawner>( ) );
    std::vector<Transform*> spawnerLocations;

    for (auto &spawner : spawners)
    {
        if (spawner->GetComponent<VineSpawner>( )->GetSpawnSegment( ) != LevelSegments::BossRoom_Phase3)
            continue;

        spawnerLocations.push_back( spawner->GetTransform( ) );
    }

    int index = 0;
    auto &vines = m_boss->GetVines( );

    for (auto location : spawnerLocations)
    {
        if (index >= vines.size())
            break;

        auto position = location->GetWorldPosition( );

        if (position != m_boss->GetHomeLocation( ))
        {
            vines[ index++ ]->GetComponent<VineAI>( )->SetHomeLocation(
                position
            );
        }
    }

    // tell them all to go under ground
    for (auto &vine : vines)
    {
        vine->GetComponent<VineAI>( )->JumpToHomeLocation( );
    }

    m_boss->JumpToHomeLocation( );
}

