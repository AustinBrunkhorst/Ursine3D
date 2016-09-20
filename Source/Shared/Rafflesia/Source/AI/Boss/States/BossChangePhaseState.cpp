/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossChangePhaseState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossChangePhaseState.h"

#include "BossAIStateMachine.h"
#include "BossAIComponent.h"
#include "LevelSegmentManagerComponent.h"

#include <World.h>
#include <Filter.h>

using namespace ursine;
using namespace ecs;

BossChangePhaseState::BossChangePhaseState(LevelSegments changeToPhase)
    : BossAIState( "Change To Phase" )
    , m_phase( changeToPhase ) { }

void BossChangePhaseState::Enter(BossAIStateMachine *machine)
{
    auto boss = machine->GetBoss( );
    auto world = boss->GetOwner( )->GetWorld( );

    auto sm = world->GetEntitiesFromFilter( Filter( ).All<LevelSegmentManager>( ) );

    if (sm.size( ))
    {
        sm[ 0 ]->GetComponent<LevelSegmentManager>( )->SetCurrentSegment( m_phase );
    }
}
