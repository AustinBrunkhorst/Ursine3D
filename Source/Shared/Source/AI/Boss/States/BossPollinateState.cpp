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
#include "PlayerIDComponent.h"
#include "HealthComponent.h"
#include "EntityAnimatorComponent.h"

#include <GhostComponent.h>
#include <AnimatorComponent.h>
#include <EntityEvent.h>

using namespace ursine;
using namespace ecs;

BossPollinateState::BossPollinateState(void)
    : BossAIState( "Boss Pollinate" )
    , m_boss( nullptr ) { }

void BossPollinateState::Enter(BossAIStateMachine *machine)
{
    // Store the boss AI
    m_boss = machine->GetBoss( );

    // play the animation
    auto animator = m_boss->GetOwner( )->GetComponentInChildren<Animator>( );

    animator->SetCurrentState( "Pollinate" );

    // subscribe to the OnAnimationFinish and OnAnimationEvent
    animator->GetOwner( )->Listener( this )
        .On( ENTITY_ANIMATION_STATE_EVENT, &BossPollinateState::onAnimationEvent );

    m_safetyShield = m_boss->GetPollinateSafetyShield( );

    m_safetyShield->GetComponent<EntityAnimator>( )->Play( );

    for (auto &pair : m_damageMap)
        pair.second = 0.0f;
}

void BossPollinateState::Update(BossAIStateMachine *machine)
{
    auto world = m_boss->GetOwner( )->GetWorld( );

    // get the players
    auto players = world->GetEntitiesFromFilter( Filter( ).All<PlayerID>( ) );

    EntityHandle p1 = nullptr, p2 = nullptr;

    for (int i = 0; i < 2; ++i)
    {
        if (players[ i ]->GetComponent<PlayerID>( )->GetID( ) == 0)
            p1 = players[ i ];
        else
            p2 = players[ i ];
    }

    // iterate through all the pairs in the safety shield's collider
    auto ghost = m_safetyShield->GetComponent<Ghost>( );
    std::vector<physics::GhostOverlappingItem> pairs;

    ghost->GetOverlappingPairs( pairs );

    for (auto &pair : pairs)
    {
        // if a player is found, set his handle to null
        if (pair.component->GetOwner( ) == p1)
            p1 = nullptr;
        else if (pair.component->GetOwner( ) == p2)
            p2 = nullptr;
    }

    // for each non null handle, deal damage to it if it's damage map value is good
    if (p1)
        tryDealDamage( p1 );

    if (p2)
        tryDealDamage( p2 );

    auto dt = Application::Instance->GetDeltaTime( );

    // decrement all damage map pairs
    for (auto &pair : m_damageMap)
        if (pair.second > 0.0f)
            pair.second -= dt;
}

void BossPollinateState::Exit(BossAIStateMachine *machine)
{
    // play idle
    auto animator = m_boss->GetOwner( )->GetComponentInChildren<Animator>( );

    // unsubscribe from everything
    animator->GetOwner( )->Listener( this )
        .Off( ENTITY_ANIMATION_STATE_EVENT, &BossPollinateState::onAnimationEvent );
}

void BossPollinateState::onAnimationEvent(EVENT_HANDLER(Entity))
{
    EVENT_ATTRS(Entity, AnimatorStateEventArgs);

    if (args->state == "Pollinate" && args->message == "Spew")
        spewPollin( );
}

void BossPollinateState::spewPollin(void)
{
    // Get the pollinate entity
    auto pollinateEntity = m_boss->GetPollinateEntity( );

    if (!pollinateEntity)
        return;

    auto pollinateTrans = pollinateEntity->GetTransform( );
    auto pollinatePosition = pollinateTrans->GetWorldPosition( );

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
            pollinatePosition
        );

        // Set it's direction of influence (u rotated by Q(worldNormal, theta))
        auto dir = SQuat( theta, worldForward ).Rotate( u );

        projectile->GetTransform( )->SetWorldRotation( 
            SQuat::LookAt( dir, worldForward )
        );

        theta += step;
    }
}

void BossPollinateState::tryDealDamage(EntityHandle entity)
{
    auto itr = m_damageMap.find( entity );

    if (itr == m_damageMap.end( ))
        m_damageMap[ entity ] = 0.0f;

    if (m_damageMap[ entity ] <= 0.0f)
    {
        entity->GetComponent<Health>( )->DealDamage(
            m_boss->GetOwner( ), m_boss->GetPollinateTickAmount( )
        );

        m_damageMap[ entity ] = m_boss->GetPollinateTickRate( );
    }
}
