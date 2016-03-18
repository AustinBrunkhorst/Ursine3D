/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossSpawnVinesState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossSpawnVinesState.h"

#include "BossAIStateMachine.h"
#include "BossAIComponent.h"

#include "VineSpawnerComponent.h"

#include <World.h>

using namespace ursine;
using namespace ecs;

BossSpawnVinesState::BossSpawnVinesState(LevelSegments spawnSegment)
    : BossAIState( "Spawn Vines" )
    , m_spawnSegment( spawnSegment )
{
}

void BossSpawnVinesState::Enter(BossAIStateMachine *machine)
{
    auto boss = machine->GetBoss( );
    auto world = boss->GetOwner( )->GetWorld( );

    auto spawners = world->GetEntitiesFromFilter( Filter( ).All<VineSpawner>( ) );
    auto vineArchetypeName = boss->GetVineArchetype( );

    for (auto &spawner : spawners)
    {
        if (spawner->GetComponent<VineSpawner>( )->GetSpawnSegment( ) != m_spawnSegment)
            continue;

        auto spawnTrans = spawner->GetTransform( );

        auto vine = world->CreateEntityFromArchetype(
            vineArchetypeName 
        );

        if (vine)
        {
            auto trans = vine->GetTransform( );

            trans->SetWorldPosition( spawnTrans->GetWorldPosition( ) );
            trans->SetWorldRotation( spawnTrans->GetWorldRotation( ) );

            boss->AddSpawnedVine( vine );
        }
    }
}
