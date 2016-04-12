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
#include "PlayerIDComponent.h"

#include <AnimatorComponent.h>
#include <EntityEvent.h>

using namespace ursine;
using namespace ecs;

BossSludgeshotState::BossSludgeshotState(int numShots, float playback, bool toIdle)
    : BossAIState( "Boss Sludgeshot" )
    , m_boss( nullptr )
    , m_finished( false )
    , m_toIdle( toIdle )
    , m_numShots( numShots )
    , m_playback( playback ) { }

void BossSludgeshotState::Enter(BossAIStateMachine *machine)
{
    // Store the boss AI
    m_boss = machine->GetBoss( );

    // play the animation
    auto animator = m_boss->GetOwner( )->GetComponentInChildren<Animator>( );

    animator->SetCurrentState( "Sludgeshot" );
    animator->SetTimeScalar( m_playback );

    // subscribe to the OnAnimationFinish and OnAnimationEvent
    animator->GetOwner( )->Listener( this )
        .On( ENTITY_ANIMATION_STATE_EVENT, &BossSludgeshotState::onAnimationEvent );

    // Set Finish to false
    m_finished = false;

    m_numShotsCounter = 0;
}

void BossSludgeshotState::Update(BossAIStateMachine *machine)
{
    if (!m_target)
        findTarget( );

    rotateTowardsTarget( );
}

void BossSludgeshotState::Exit(BossAIStateMachine *machine)
{
    // play idle
    auto animator = m_boss->GetOwner( )->GetComponentInChildren<Animator>( );

    // unsubscribe from everything
    animator->GetOwner( )->Listener( this )
        .Off( ENTITY_ANIMATION_STATE_EVENT, &BossSludgeshotState::onAnimationEvent );

    animator->SetTimeScalar( 1.0f );

    if (m_toIdle)
        animator->SetCurrentState( "Idle" );
}

void BossSludgeshotState::onAnimationEvent(EVENT_HANDLER(Entity))
{
    EVENT_ATTRS(Entity, AnimatorStateEventArgs);

    if (args->state == "Sludgeshot" && args->message == "Shoot")
        shootSludge( );

    ++m_numShotsCounter;

    if (m_numShotsCounter >= m_numShots)
        m_finished = true;
}

void BossSludgeshotState::shootSludge(void)
{
    // Get the sludgeshot entity
    auto sludgeshotEntity = m_boss->GetSludgeshotEntity( );

    if (!sludgeshotEntity)
        return;

    if (!m_target)
        findTarget( );

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

    sludgeshot->SetTargetPosition( 
        m_target->GetTransform( )->GetWorldPosition( )
    );

    sludgeshot->SetTotalTimeOfAnimation( m_boss->GetSludgeshotAnimationTime( ) );

    sludgeshot->InitializeComponents( );

    // Find new target
    findTarget( );
}

void BossSludgeshotState::findTarget(void)
{
    auto world = m_boss->GetOwner( )->GetWorld( );
    auto players = world->GetEntitiesFromFilter( Filter( ).All<PlayerID>( ) );
    auto size = players.size( );

    m_target = players[ rand( ) % size ];
}

void BossSludgeshotState::rotateTowardsTarget(void)
{
    if (!m_target)
        return;

    auto bossTrans = m_boss->GetOwner( )->GetTransform( );
    auto targetTrans = m_target->GetTransform( );

    auto targetPos = targetTrans->GetWorldPosition( );
    auto bossPos = bossTrans->GetWorldPosition( );

    targetPos.Y( ) = bossPos.Y( );

    bossTrans->LookAt( targetPos, m_boss->GetSeedshotTurnSpeed( ) );
}
