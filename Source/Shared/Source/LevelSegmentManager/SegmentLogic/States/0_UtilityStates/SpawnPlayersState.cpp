/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SpawnPlayerState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "SpawnPlayersState.h"

#include "SegmentLogicStateIncludes.h"

#include "PlayerIDComponent.h"
#include "PlayerSpawnPointComponent.h"

using namespace ursine;
using namespace ecs;

SpawnPlayersState::SpawnPlayersState(void)
    : SegmentLogicState( "Spawn Players" )
{
}

void SpawnPlayersState::Enter(SegmentLogicStateMachine *machine)
{
    auto segmentManager = machine->GetSegmentManager( );
    auto world = segmentManager->GetOwner( )->GetWorld( );

    auto players = world->GetEntitiesFromFilter( Filter( ).All<PlayerID>( ) );

    if (players.size( ) == 0)
    {
        auto spawnPoints = world->GetEntitiesFromFilter( Filter( ).All<PlayerSpawnPoint>( ) );

        for (auto &spawnPoint : spawnPoints)
        {
            auto point = spawnPoint->GetComponent<PlayerSpawnPoint>( );
            auto archetype = point->GetPlayerArchetype( );

            auto player = world->CreateEntityFromArchetype(
                WORLD_ARCHETYPE_PATH + archetype + ".uatype", archetype
            );

            auto spawnTrans = spawnPoint->GetTransform( );
            auto playerTrans = player->GetTransform( );

            playerTrans->SetWorldPosition(
                spawnTrans->GetWorldPosition( )
            );

            playerTrans->SetWorldRotation(
                spawnTrans->GetWorldRotation( )
            );
        }
    }
}
