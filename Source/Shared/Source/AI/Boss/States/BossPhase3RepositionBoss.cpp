/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossPhase3RepositionBoss.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossPhase3RepositionBoss.h"

#include "Phase3BossSpawnComponent.h"
#include "BossAIStateMachine.h"
#include "BossAIComponent.h"
#include "VineSpawnerComponent.h"

#include <World.h>

using namespace ursine;
using namespace ecs;

BossPhase3RepositionBoss::BossPhase3RepositionBoss(bool onlyBossSpawn, bool goToCenterPlanter)
    : BossAIState( "Reposition Boss" )
    , m_onlyBossSpawn( onlyBossSpawn )
    , m_goToCenterPlanter( goToCenterPlanter ) { }

void BossPhase3RepositionBoss::Enter(BossAIStateMachine *machine)
{
    auto ai = machine->GetBoss( );
    auto boss = ai->GetOwner( )->GetTransform( );
    auto world = boss->GetOwner( )->GetWorld( );

    if (m_goToCenterPlanter)
    {
        boss->SetWorldPosition( SVec3( 0.0f, 8.9f, 0.0f ) );
        boss->SetWorldRotation( SQuat::Identity( ) );
    }
    else if (m_onlyBossSpawn)
    {
        auto spawnPoints = world->GetEntitiesFromFilter( Filter( ).All<Phase3BossSpawn>( ) );

        UAssert( spawnPoints.size( ), "Error: There isn't a spawn point." );

        auto trans = spawnPoints[ 0 ]->GetTransform( );
        auto position = trans->GetWorldPosition( );
        auto rotation = trans->GetWorldRotation( );

        boss->SetWorldPosition( position );
        boss->SetWorldRotation( rotation );
    }
    else
    {
        auto spawners = world->GetEntitiesFromFilter( Filter( ).All<VineSpawner>( ) );
        std::vector<Transform *> spawnPoints;

        for (auto &spawner : spawners)
        {
            if (spawner->GetComponent<VineSpawner>( )->GetSpawnSegment( ) != LevelSegments::BossRoom_Phase3)
                continue;

            spawnPoints.push_back( spawner->GetTransform( ) );
        }

        bool found = false;

        do
        {
            auto point = spawnPoints[ rand( ) % spawnPoints.size( ) ];
            auto position = point->GetWorldPosition( );

            if (position == ai->GetHomeLocation( ))
                continue;

            boss->SetWorldPosition( position );
            boss->SetWorldRotation( point->GetWorldRotation( ) );

            found = true;

        } while (!found);
    }
}
