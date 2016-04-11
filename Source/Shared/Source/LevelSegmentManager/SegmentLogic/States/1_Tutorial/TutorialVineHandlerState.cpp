/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TutorialVineHandlerState.cpp
**
** Author:
** - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "Precompiled.h"

#include "TutorialVineHandlerState.h"

#include "VineSpawnerComponent.h"
#include "SegmentLogicStateMachine.h"
#include "LevelSegmentManagerComponent.h"

#include <ResourceReference.h>
#include <World.h>

using namespace ursine;
using namespace ecs;
using namespace resources;

TutorialVineHandlerState::TutorialVineHandlerState(ResourceReference vineArchetype)
    : SegmentLogicState( "Vine Handler For Tutorial" )
    , m_vineCount( 0 )
    , m_vineArchetype( vineArchetype )
    , m_machine( nullptr ) { }

void TutorialVineHandlerState::Enter(SegmentLogicStateMachine *machine)
{
    m_vineCount = 0;
    m_machine = machine;

    auto sm = machine->GetSegmentManager( );
    auto world = sm->GetOwner( )->GetWorld( );

    // Get all spawners
    auto spawns = world->GetEntitiesFromFilter( Filter( ).All<VineSpawner>( ) );

    for (auto &spawn : spawns)
    {
        // spawn vine archetype at all vine spawners
        auto vine = world->CreateEntityFromArchetype( m_vineArchetype );

        UAssert( vine, "Error bro: this vine archetype is not valid dogg.  Look at TutorialResources component." );

        vine->GetTransform( )->SetWorldPosition(
            spawn->GetTransform( )->GetWorldPosition( ) 
        );

        // subscribe to each one's on death event
        auto health = vine->GetComponent<Health>( );

        UAssert( health, "Error: the vine must have a health component." );

        health->Listener( this )
            .On( HEALTH_ZERO, &TutorialVineHandlerState::onVineDeath );

        // set the counter
        ++m_vineCount;
    }
}

void TutorialVineHandlerState::onVineDeath(EVENT_HANDLER(Health))
{
    --m_vineCount;

    if (m_vineCount <= 0)
    {
        m_machine->GetSegmentManager( )->SetCurrentSegment( LevelSegments::Tut_DoorOpenTutorial );
    }
}
