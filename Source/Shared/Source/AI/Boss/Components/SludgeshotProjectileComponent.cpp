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

#include "PlayerIDComponent.h"

#include <LocalTimerManager.h>
#include <SystemManager.h>
#include <DebugSystem.h>
#include <SweptControllerComponent.h>
#include <HealthComponent.h>
#include <World.h>

NATIVE_COMPONENT_DEFINITION(SludgeshotProjectile);

using namespace ursine;
using namespace ecs;

SludgeshotProjectile::SludgeshotProjectile(void)
    : BaseComponent( )
    , m_time( 0.0f )
    , m_emitionRate( 0.0f )
    , m_range( 10.0f )
    , m_damage( 50.0f )
    , m_interval( 1.0f )
    , m_impulse( 100.0f ) { }

SludgeshotProjectile::~SludgeshotProjectile(void)
{
    auto animator = GetOwner( )->GetComponent<EntityAnimator>( );

    if (animator)
        animator->Listener( this )
            .Off( EntityAnimatorEvent::FinishedAnimating, &SludgeshotProjectile::onAnimationCompleted );

    auto world = GetOwner( )->GetWorld( );
    
    if (world)
        world->Listener( this )
            .Off( WORLD_UPDATE, &SludgeshotProjectile::onUpdate );
}

float SludgeshotProjectile::GetRange(void) const
{
    return m_range;
}

void SludgeshotProjectile::SetRange(float range)
{
    m_range = range;

    NOTIFY_COMPONENT_CHANGED( "range", m_range );
}

float SludgeshotProjectile::GetDamage(void) const
{
    return m_damage;
}

void SludgeshotProjectile::SetDamage(float damage)
{
    m_damage = damage;

    NOTIFY_COMPONENT_CHANGED( "damage", m_damage );
}

float SludgeshotProjectile::GetDamageInterval(void) const
{
    return m_interval;
}

void SludgeshotProjectile::SetDamageInterval(float interval)
{
    m_interval = interval;

    NOTIFY_COMPONENT_CHANGED( "damageInterval", m_interval );
}

float SludgeshotProjectile::GetImpulse(void) const
{
    return m_impulse;
}

void SludgeshotProjectile::SetImpulse(float impulse)
{
    m_impulse = impulse;

    NOTIFY_COMPONENT_CHANGED( "impulse", m_impulse );
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

    // Make sure the child has an emitter
    auto emitter = owner->GetComponentInChildren<ParticleEmitter>( );

    UAssert( emitter, "Error: A child entity must have a ParticleEmitter component." );

    m_emitionRate = emitter->GetEmitRate( );

    emitter->SetEmitRate( 0 );

    // Setup the animator
    auto animator = owner->GetComponent<EntityAnimator>( );

    animator->SetSmoothPath( true );
    animator->SetLoopAnimation( false );

    // Calculate the start, middle, and end positions
    auto start = owner->GetTransform( )->GetWorldPosition( );
    auto end = m_target;
    auto dir = end - start;
    auto middle = start + SVec3( dir.X( ) * 0.8f, -dir.Y( ) * 0.2f, dir.X( ) * 0.8f );

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

    childEmitter->SetEmitRate( m_emitionRate );
    childEmitter->ResetSpawnCount( );

    // Also remove the health component
    // TODO:

    // After one second, delete the entity
    owner->GetTimers( ).Create( TimeSpan::FromSeconds( 1.0f ) )
        .Completed( [=] {
            owner->Delete( );
        } );

    sender->Listener( this )
        .Off( EntityAnimatorEvent::FinishedAnimating, &SludgeshotProjectile::onAnimationCompleted );

    owner->GetWorld( )->Listener( this )
        .On( WORLD_UPDATE, &SludgeshotProjectile::onUpdate );
}

void SludgeshotProjectile::onUpdate(EVENT_HANDLER(World))
{
    // get all players
    auto  world = GetOwner( )->GetWorld( );
    auto players = world->GetEntitiesFromFilter( Filter( ).All<PlayerID>( ) );
    auto ourPosition = GetOwner( )->GetTransform( )->GetWorldPosition( );

    // iterate through them
    for (auto &player : players)
    {
        auto playerPos = player->GetTransform( )->GetWorldPosition( );

        // if in range
        if (SVec3::DistanceSquared( playerPos, ourPosition ) <= m_range * m_range)
        {
            // if damage map value is less than zero, damage dat bitch
            auto itr = m_damageMap.find( player );

            if (itr == m_damageMap.end( ) || m_damageMap[ player ] <= 0.0f)
            {
                player->GetComponent<Health>( )->DealDamage( GetOwner( ), m_damage );

                m_damageMap[ player ] = m_interval;

                // apply an impulse away from ourselves
                player->GetComponent<SweptController>( )->AddImpulse(
                    SVec3::Normalize( playerPos - ourPosition ) * m_impulse
                );
            }
        }
    }

    auto dt = Application::Instance->GetDeltaTime( );

    for (auto &pair : m_damageMap)
    {
        pair.second -= dt;
    }
}

#if defined(URSINE_WITH_EDITOR)

void SludgeshotProjectile::drawRange(void)
{
    auto world = GetOwner( )->GetWorld( );

    if (!world)
        return;

    auto debug = world->GetEntitySystem<DebugSystem>( );

    auto pos = GetOwner( )->GetTransform( )->GetWorldPosition( );

    debug->DrawSphere( pos, m_range, Color::Gold, 3.0f, true );
}

#endif
