/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BossSludgeshotState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossSludgeshotState.h"

#include "BossAIStateMachine.h"
#include "BossAIComponent.h"
#include "SludgeshotProjectileComponent.h"

#include <AnimatorComponent.h>
#include <EntityEvent.h>

using namespace ursine;
using namespace ecs;

BossSludgeshotState::BossSludgeshotState(void)
    : BossAIState( "Boss Sludgeshot" )
    , m_boss( nullptr )
    , m_finished( false ) { }

void BossSludgeshotState::Enter(BossAIStateMachine *machine)
{
    // Store the boss AI
    m_boss = machine->GetBoss( );

    // play the animation
    auto animator = m_boss->GetOwner( )->GetComponentInChildren<Animator>( );

    animator->SetCurrentState( "Sludgeshot" );

    // subscribe to the OnAnimationFinish and OnAnimationEvent
    animator->GetOwner( )->Listener( this )
        .On( ENTITY_ANIMATION_STATE_EVENT, &BossSludgeshotState::onAnimationEvent )
        .On( ENTITY_ANIMATION_FINISH, &BossSludgeshotState::onAnimationFinish );

    // Set Finish to false
    m_finished = false;
}

void BossSludgeshotState::Exit(BossAIStateMachine *machine)
{
    // play idle
    auto animator = m_boss->GetOwner( )->GetComponentInChildren<Animator>( );

    animator->SetCurrentState( "Idle" );

    // unsubscribe from everything
    animator->GetOwner( )->Listener( this )
        .Off( ENTITY_ANIMATION_STATE_EVENT, &BossSludgeshotState::onAnimationEvent )
        .Off( ENTITY_ANIMATION_FINISH, &BossSludgeshotState::onAnimationFinish );
}

void BossSludgeshotState::onAnimationEvent(EVENT_HANDLER(Entity))
{
    EVENT_ATTRS(Entity, AnimatorStateEventArgs);

    if (args->state == "Sludgeshot" && args->message == "Shoot")
        shootSludge( );
}

void BossSludgeshotState::onAnimationFinish(EVENT_HANDLER(Entity))
{
    m_finished = true;
}

void BossSludgeshotState::shootSludge(void)
{
    // Get the sludgeshot entity
    auto sludgeshotEntity = m_boss->GetSludgeshotEntity( );

    if (!sludgeshotEntity)
        return;

    auto sludgeshotTrans = sludgeshotEntity->GetTransform( );
    auto sludgeshotPosition = sludgeshotTrans->GetWorldPosition( );

    auto projectileArchetype = m_boss->GetSludgeshotArchetype( );

    auto world = m_boss->GetOwner( )->GetWorld( );

    auto projectile = world->CreateEntityFromArchetype(
        projectileArchetype
    );

    if (!projectile)
        return;

    projectile->GetTransform()->SetWorldPosition(
        sludgeshotPosition
    );

    // The projectile must have this component
    UAssert(
        projectile->HasComponent<SludgeshotProjectile>( ),
        "Error: The sludgeshot projectile must of have the \"SludgeshotProjectile\" component."
    );

    auto sludgeshot = projectile->GetComponent<SludgeshotProjectile>( );

    // TODO: make this an actual target
    sludgeshot->SetTargetPosition( 
        m_boss->GetOwner( )->GetTransform( )->GetWorldPosition( ) + SVec3::UnitX( ) * 50.0f 
    );

    sludgeshot->SetTotalTimeOfAnimation( m_boss->GetSludgeshotAnimationTime( ) );

    sludgeshot->InitializeComponents( );
}
