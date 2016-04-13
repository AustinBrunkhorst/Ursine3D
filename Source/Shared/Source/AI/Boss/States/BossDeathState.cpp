/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossDeathState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossDeathState.h"

#include "BossAIStateMachine.h"
#include "BossAIComponent.h"

#include <AnimatorComponent.h>

using namespace ursine;
using namespace ecs;

BossDeathState::BossDeathState(void)
    : BossAIState( "Death" ) { }

void BossDeathState::Enter(BossAIStateMachine *machine)
{
    auto animator = machine->GetBoss( )->GetOwner( )->GetComponentInChildren<Animator>( );
    animator->SetCurrentState( "Death" );
    animator->SetPlaying( true );
}
