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

#include <World.h>

using namespace ursine;
using namespace ecs;

BossPhase3RepositionBoss::BossPhase3RepositionBoss(void)
    : BossAIState( "Reposition Boss" ) { }

void BossPhase3RepositionBoss::Enter(BossAIStateMachine *machine)
{
    auto boss = machine->GetBoss( )->GetOwner( )->GetTransform( );
    auto world = boss->GetOwner( )->GetWorld( );

    auto spawnPoints = world->GetEntitiesFromFilter( Filter( ).All<Phase3BossSpawn>( ) );

    UAssert( spawnPoints.size( ), "Error: There isn't a spawn point." );

    auto trans = spawnPoints[ 0 ]->GetTransform( );
    auto position = trans->GetWorldPosition( );
    auto rotation = trans->GetWorldRotation( );

    boss->SetWorldPosition( position );
    boss->SetWorldRotation( rotation );
}
