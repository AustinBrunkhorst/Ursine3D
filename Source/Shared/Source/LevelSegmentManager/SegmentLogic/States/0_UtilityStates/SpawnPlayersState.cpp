\/* ---------------------------------------------------------------------------
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

#include <CameraComponent.h>

using namespace ursine;
using namespace ecs;

SpawnPlayersState::SpawnPlayersState(bool repositionIfPresent, bool turnOffCameras)
    : SegmentLogicState( "Spawn Players" )
    , m_repositionIfPresent( repositionIfPresent )
    , m_turnOffCameras( turnOffCameras )
{
}

void SpawnPlayersState::Enter(SegmentLogicStateMachine *machine)
{
    auto segmentManager = machine->GetSegmentManager( );
    auto world = segmentManager->GetOwner( )->GetWorld( );

    auto players = world->GetEntitiesFromFilter( Filter( ).All<PlayerID>( ) );
    int index = 0;

    bool spawn = players.size( ) == 0;

    auto spawnPoints = world->GetEntitiesFromFilter( Filter( ).All<PlayerSpawnPoint>( ) );

    UAssert( spawnPoints.size( ) != 0, "Error: This state requires spawn points to be present in the level." );

    // If the number of spawn points found are greater than two, find the two that are nearest to our current segment
    if (spawnPoints.size( ) > 2)
    {
        auto cur = segmentManager->GetCurrentSegment( );

        auto spawnPoint1 = spawnPoints[ 0 ]->GetComponent<PlayerSpawnPoint>( );
        auto spawnPoint2 = spawnPoints[ 1 ]->GetComponent<PlayerSpawnPoint>( );

        for (size_t i = 2; i < spawnPoints.size( ); ++i)
        {
            auto newSpawn = spawnPoints[ i ]->GetComponent<PlayerSpawnPoint>( );

            if (newSpawn->GetSpawnSegment( ) > spawnPoint1->GetSpawnSegment( ))
                spawnPoint1 = newSpawn;
            else if (newSpawn->GetSpawnSegment( ) > spawnPoint2->GetSpawnSegment( ))
                spawnPoint2 = newSpawn;
        }

        spawnPoints = { spawnPoint1->GetOwner( ), spawnPoint2->GetOwner( ) };
    }

    for (auto &spawnPoint : spawnPoints)
    {
        auto point = spawnPoint->GetComponent<PlayerSpawnPoint>( );
        auto player = spawn ? EntityHandle::Invalid( ) : players[ index++ ];

        if (spawn)
        {
            auto archetype = point->GetPlayerArchetype( );

            player = world->CreateEntityFromArchetype(
                WORLD_ARCHETYPE_PATH + archetype + ".uatype", archetype
            );

            if (player->GetComponent<PlayerID>( )->GetID( ) == 0)
                segmentManager->m_player1 = player;
            else
                segmentManager->m_player2 = player;
        }

        if (spawn || m_repositionIfPresent)
        {
            auto spawnTrans = spawnPoint->GetTransform( );
            auto playerTrans = player->GetTransform( );

            playerTrans->SetWorldPosition(
                spawnTrans->GetWorldPosition( )
            );

            playerTrans->SetWorldRotation(
                spawnTrans->GetWorldRotation( )
            );
        }

        auto cam = player->GetComponentInChildren<Camera>( );

        cam->SetActive( !m_turnOffCameras );
    }
}
