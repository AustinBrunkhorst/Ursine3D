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

#include <ParticleEmitterComponent.h>
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

    boss->GetPollinateSmogEntity( )->GetComponent<ParticleEmitter>( )->SetEmitRate( 0.0f );
}

void BossDeathState::Update(BossAIStateMachine *machine)
{
    auto boss = machine->GetBoss( );
    auto trans = boss->GetOwner( )->GetTransform( );

    if (trans->GetWorldRotation( ) != SQuat::Identity( ))
    {
        trans->LookAt( SVec3( 0.0f, 0.0f, 20.0f ), 45.0f );
    }
}
