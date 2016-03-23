/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BossPollinateState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossPollinateState.h"
#include "BossAIComponent.h"
#include "PollinateProjectileComponent.h"

#include <AnimatorComponent.h>
#include <EntityEvent.h>

using namespace ursine;
using namespace ecs;

BossPollinateState::BossPollinateState(void)
    : BossAIState( "Boss Pollinate" )
    , m_boss( nullptr )
    , m_finished( false ) { }

void BossPollinateState::Enter(BossAIStateMachine *machine)
{
    // Store the boss AI
    m_boss = machine->GetBoss( );

    // play the animation
    auto animator = m_boss->GetOwner( )->GetComponentInChildren<Animator>( );

    animator->SetCurrentState( "Pollinate" );

    // subscribe to the OnAnimationFinish and OnAnimationEvent
    animator->GetOwner( )->Listener( this )
        .On( ENTITY_ANIMATION_STATE_EVENT, &BossPollinateState::onAnimationEvent )
        .On( ENTITY_ANIMATION_FINISH, &BossPollinateState::onAnimationFinish );

    // Set Finish to false
    m_finished = false;
}

void BossPollinateState::Exit(BossAIStateMachine *machine)
{
    // play idle
    auto animator = m_boss->GetOwner( )->GetComponentInChildren<Animator>( );

    animator->SetCurrentState( "Idle" );

    // unsubscribe from everything
    animator->GetOwner( )->Listener( this )
        .Off( ENTITY_ANIMATION_STATE_EVENT, &BossPollinateState::onAnimationEvent )
        .Off( ENTITY_ANIMATION_FINISH, &BossPollinateState::onAnimationFinish );
}

void BossPollinateState::onAnimationEvent(EVENT_HANDLER(Entity))
{
    EVENT_ATTRS(Entity, AnimatorStateEventArgs);

    if (args->state == "Pollinate" && args->message == "Spew")
        spewPollin( );
}

void BossPollinateState::onAnimationFinish(EVENT_HANDLER(Entity))
{
    m_finished = true;
}

void BossPollinateState::spewPollin(void)
{
    // Get the pollinate entity
    auto pollinateEntity = m_boss->GetPollinateEntity( );

    if (!pollinateEntity)
        return;

    auto pollinateTrans = pollinateEntity->GetTransform( );
    auto pollinatePosition = pollinateTrans->GetWorldPosition( );

    // get the angle
    auto angle = m_boss->GetMaxPollinateSpreadAngle( );

    // get the world forward vector
    auto focus = pollinateTrans->ToWorld(
        m_boss->GetPollinateLocalForward( )
    );

    auto worldForward = SVec3::Normalize( focus - pollinatePosition );

    // Get the perpendicular vectors
    SVec3 u, v;

    worldForward.GenerateOrthogonalVectors( u, v );

    // get the pollinate archetype
    auto projectileArchetype = m_boss->GetPollinateArchetype( );

    auto numberToSpawn = m_boss->GetPollinateProjectileCount( );

    auto world = pollinateEntity->GetWorld( );

    float theta = 0.0f;
    float step = 360.0f / numberToSpawn;

    for (int i = 0; i < numberToSpawn; ++i)
    {
        // spawn the archetype
        auto projectile = world->CreateEntityFromArchetype(
            projectileArchetype
        );

        if (!projectile)
            return;

        // Set the spawn position
        projectile->GetTransform( )->SetWorldPosition(
            pollinateTrans->GetWorldPosition( )
        );

        // The projectile must have this component
        UAssert(
            projectile->HasComponent<PollinateProjectile>( ),
            "Error: The pollinate projectile must of have the \"PollinateProjectile\" component."
        );

        auto pollinate = projectile->GetComponent<PollinateProjectile>( );

        // Set it's direction of influence (u rotated by Q(worldNormal, theta))
        auto dir = SQuat( theta, worldForward ).Rotate( u );

        pollinate->SetDirection( dir );

        // Set the gravity
        pollinate->SetGravity( m_boss->GetPollinateGravity( ) );

        // Set the distance
        pollinate->SetSpreadDistance( m_boss->GetPollinateSpreadDistance( ) );

        // Set the time it takes to get there
        pollinate->SetSpreadTime( m_boss->GetPollinateSpreadTime( ) );

        // Set the lifetime
        pollinate->SetLifeTime( m_boss->GetPollinateProjectileLifeTime( ) );

        pollinate->InitializeComponents( );

        theta += step;
    }
}
