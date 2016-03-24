/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RepositionPlayersAndCloseDoorState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "RepositionPlayersAndCloseDoorState.h"
#include "SegmentLogicStateMachine.h"
#include "LevelSegmentManagerComponent.h"
#include "PlayerIDComponent.h"
#include "PlayerSpawnPointComponent.h"
#include "EntityAnimatorGroupComponent.h"
#include "BossRoomResourcesComponent.h"

#include <World.h>

using namespace ursine;
using namespace ecs;

RepositionPlayersAndCloseDoorState::RepositionPlayersAndCloseDoorState(void)
    : SegmentLogicState( "Reposition Players And Close Door" ) { }

void RepositionPlayersAndCloseDoorState::Enter(SegmentLogicStateMachine *machine)
{
    auto segmentManager = machine->GetSegmentManager( );
    auto world = segmentManager->GetOwner( )->GetWorld( );

    auto players = world->GetEntitiesFromFilter( Filter( ).All<PlayerID>( ) );

    UAssert( players.size( ) == 2, "What's up with this?  Where are the players?" );
    
    auto spawnPoints = world->GetEntitiesFromFilter( Filter( ).All<PlayerSpawnPoint>( ) );

    auto playerItr = players.begin( );

    for (auto &spawnPoint : spawnPoints)
    {
        auto sp = spawnPoint->GetComponent<PlayerSpawnPoint>( );

        if (sp->GetSpawnSegment( ) == LevelSegments::BossRoom_Phase1)
        {
            auto playerTrans = playerItr->Get( )->GetTransform( );
            auto spTrans = spawnPoint->GetTransform( );

            playerTrans->SetWorldPosition(
                spTrans->GetWorldPosition( )
            );

            playerTrans->SetWorldRotation(
                spTrans->GetWorldRotation( )
            );

            if (playerItr == players.begin() + 1)
                break;

            ++playerItr;
        }
    }

    // get the boss door and close it
    auto resources = segmentManager->GetOwner( )->GetComponent<BossRoomResources>( );
    auto bossDoor = world->GetEntityFromName( resources->bossDoorEntityName );

    UAssert( bossDoor, "What the fuck duder, where's the door?" );

    bossDoor->GetComponent<EntityAnimatorGroup>( )->StopGroupAnimators( );
}
