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
#include "HealthComponent.h"
#include "PlayerIDComponent.h"

#include <ParticleEmitterComponent.h>
#include <AnimatorComponent.h>
#include <LocalTimerManager.h>

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

    auto emitter = boss->GetOwner( )->GetComponent<AudioEmitter>( );
    emitter->PushEvent( boss->GetShieldBreakSfx( ) );
    emitter->PushEvent( boss->GetDeathScream( ) );

    boss->GetOwner( )->GetTimers( ).Create( TimeSpan::FromSeconds( 2.0f ) )
        .Repeat( 4 ).Repeated( [=] {
            emitter->PushEvent( boss->GetStageScream( ) );
        } );

    boss->GetPollinateSmogEntity( )->GetComponent<ParticleEmitter>( )->SetEmitRate( 0.0f );

    // Set both players to invulnerable
    auto players = boss->GetOwner( )->GetWorld( )->GetEntitiesFromFilter( Filter( ).All<PlayerID>( ) );

    for (auto &player : players)
        player->GetComponent<Health>( )->SetInvulnerable( true );
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
