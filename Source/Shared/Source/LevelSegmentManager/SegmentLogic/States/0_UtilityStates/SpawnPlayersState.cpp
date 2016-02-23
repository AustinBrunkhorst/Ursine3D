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

    for (auto &spawnPoint : spawnPoints)
    {
        auto point = spawnPoint->GetComponent<PlayerSpawnPoint>( );
        Entity *player = spawn ? nullptr : players[ index++ ];

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
