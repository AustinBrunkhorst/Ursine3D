/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossDazedState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossDazedState.h"

#include "BossAIStateMachine.h"
#include "BossAIComponent.h"

#include <AnimatorComponent.h>

using namespace ursine;
using namespace ecs;

BossDazedState::BossDazedState(void)
    : BossAIState( "Boss Dazed" ) { }

void BossDazedState::Enter(BossAIStateMachine *machine)
{
    auto boss = machine->GetBoss( );

    auto animator = boss->GetOwner( )->GetComponentInChildren<Animator>( );

    animator->SetCurrentState( "Idle" );
    animator->SetTimeScalar( 0.5f );
    animator->SetPlaying( true );
}

void BossDazedState::Exit(BossAIStateMachine *machine)
{
    auto boss = machine->GetBoss( );

    auto animator = boss->GetOwner( )->GetComponentInChildren<Animator>( );

    animator->SetTimeScalar( 1.0f );
}
