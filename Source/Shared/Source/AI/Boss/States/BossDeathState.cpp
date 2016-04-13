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
#include "ShieldFXComponent.h"

#include <AnimatorComponent.h>

using namespace ursine;
using namespace ecs;

BossDeathState::BossDeathState(void)
    : BossAIState( "Death" ) { }

void BossDeathState::Enter(BossAIStateMachine *machine)
{
    auto boss = machine->GetBoss( );

    auto animator = boss->GetOwner( )->GetComponentInChildren<Animator>( );
    animator->SetCurrentState( "Death" );
    animator->SetPlaying( true );

    boss->GetPollinateSafetyShield( )->GetComponentInChildren<ShieldFX>( )->DestroyShield( );

    boss->GetOwner( )->GetComponent<AudioEmitter>( )->PushEvent(
        boss->GetShieldBreakSfx( )
    );
}
