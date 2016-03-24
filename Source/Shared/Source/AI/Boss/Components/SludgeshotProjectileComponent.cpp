/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** SludgeshotProjectileComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "SludgeshotProjectileComponent.h"

#include <LocalTimerManager.h>
#include <GhostComponent.h>

NATIVE_COMPONENT_DEFINITION(SludgeshotProjectile);

using namespace ursine;
using namespace ecs;

SludgeshotProjectile::SludgeshotProjectile(void)
    : BaseComponent( )
    , m_time( 0.0f ) { }

SludgeshotProjectile::~SludgeshotProjectile(void)
{
    auto animator = GetOwner( )->GetComponent<EntityAnimator>( );

    if (animator)
        animator->Listener( this )
            .Off( EntityAnimatorEvent::FinishedAnimating, &SludgeshotProjectile::onAnimationCompleted );
}

void SludgeshotProjectile::SetTargetPosition(const ursine::SVec3& target)
{
    m_target = target;
}

void SludgeshotProjectile::SetTotalTimeOfAnimation(float time)
{
    m_time = time;
}

void SludgeshotProjectile::InitializeComponents(void)
{
    auto owner = GetOwner( );

    // Make sure the child has a collider
    auto ghost = owner->GetComponentInChildren<Ghost>( );

    UAssert( ghost, "Error: A child entity must have a ghost component." );

    // Make sure the child has an emitter
    auto emitter = owner->GetComponentInChildren<ParticleEmitter>( );

    UAssert( emitter, "Error: A child entity must have a ParticleEmitter component." );

    // TODO: Make sure this emitter doesn't do anything on spawn

    // Setup the animator
    auto animator = owner->GetComponent<EntityAnimator>( );

    animator->SetSmoothPath( true );
    animator->SetLoopAnimation( false );

    // Calculate the start, middle, and end positions
    auto start = owner->GetTransform( )->GetWorldPosition( );
    auto end = m_target;
    auto dir = end - start;
    auto middle = start + SVec3( dir.X( ) * 0.8f, dir.Y( ) * 0.2f, dir.X( ) * 0.8f );

    auto &keyFrames = animator->keyFrames;

    EntityKeyFrame frame;

    frame.delta = m_time / 3.0f;
    frame.positionKey = true;
    frame.rotationKey = false;
    frame.scaleKey = false;

    frame.position = start;

    keyFrames.Push( frame );

    frame.position = middle;

    keyFrames.Push( frame );

    frame.position = end;

    keyFrames.Push( frame );

    animator->JumpToStart( );
    animator->Play( );

    animator->Listener( this )
        .On( EntityAnimatorEvent::FinishedAnimating, &SludgeshotProjectile::onAnimationCompleted );

    // Make sure "Listen to children" is unchecked
    auto damage = owner->GetComponent<DamageOnCollide>( );

    damage->SetListenToChildren( false );
}

void SludgeshotProjectile::onAnimationCompleted(EVENT_HANDLER(EntityAnimator))
{
    EVENT_ATTRS(EntityAnimator, EventArgs);

    auto owner = GetOwner( );

    // on animation finish, tell the current emitter to stop emitting
    // and tell teh damage on collide to listen to children.
    auto emitter = owner->GetComponent<ParticleEmitter>( );

    emitter->SetEmitRate( 0.0f );

    auto childEmitter = owner->GetComponentInChildren<ParticleEmitter>( );

    childEmitter->ResetSpawnCount( );

    auto damageDealer = owner->GetComponent<DamageOnCollide>( );

    damageDealer->SetListenToChildren( true );

    // Also remove the health component
    // TODO:

    // After one second, delete the entity
    owner->GetTimers( ).Create( TimeSpan::FromSeconds( 1.0f ) )
        .Completed( [=] {
            owner->Delete( );
        } );

    sender->Listener( this )
        .Off( EntityAnimatorEvent::FinishedAnimating, &SludgeshotProjectile::onAnimationCompleted );
}
