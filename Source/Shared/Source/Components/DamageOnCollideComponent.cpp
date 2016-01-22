/* ---------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** DealDamageComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "DamageOnCollideComponent.h"
#include "HealthComponent.h"
#include <EntityEvent.h>
#include <CollisionEventArgs.h>
#include <stack>

NATIVE_COMPONENT_DEFINITION( DamageOnCollide );

DamageOnCollide::DamageOnCollide( void )
    : BaseComponent( )
    , m_damageToApply( 1.0f )
    , m_critModifier( 2.0f )
    , m_damageInterval( 0.5f )
    , m_deleteOnCollision( true )
    , m_deleted ( false )
{
}

DamageOnCollide::~DamageOnCollide( void )
{
}

void DamageOnCollide::OnInitialize( void )
{
    GetOwner( )->Listener( this )
        .On( ursine::ecs::ENTITY_COLLISION_PERSISTED, &DamageOnCollide::ApplyDamage );
}

float DamageOnCollide::GetDamageToApply( void ) const
{
    return m_damageToApply;
}

void DamageOnCollide::SetDamageToApply( const float damage )
{
    m_damageToApply = damage;
}

float DamageOnCollide::GetCritModifier( void ) const
{
    return m_critModifier;
}

void DamageOnCollide::SetCritModifier( const float modifier )
{
    m_critModifier = modifier;
}

float DamageOnCollide::GetDamageInterval( void ) const
{
    return m_damageInterval;
}

void DamageOnCollide::SetDamageInterval( const float damageInterval )
{
    m_damageInterval = damageInterval;
}

bool DamageOnCollide::GetDeleteOnCollision( void ) const
{
    return m_deleteOnCollision;
}

void DamageOnCollide::SetDeleteOnCollision( const bool state )
{
    m_deleteOnCollision = state;
}

void DamageOnCollide::ApplyDamage( EVENT_HANDLER( ursine::ecs::ENTITY_COLLISION_PERSISTED ) )
{
    EVENT_ATTRS( ursine::ecs::Entity, ursine::physics::CollisionEventArgs );

    Health* healthComp = args->otherEntity->GetComponent<Health>( );

    // does object colliding with have a health component
    if ( healthComp && m_damageTimeMap[ args->otherEntity->GetID( ) ] == NULL)
    {
        // apply damage to other object
        healthComp->DealDamage( m_damageToApply );

        // delete this projectile if delete on collision flag is set
        if ( m_deleteOnCollision )
        {
            GetOwner( )->Delete( );
            m_deleted = true;
        }

        // add other object to damage interval map
        else
        {
            m_damageTimeMap[ args->otherEntity->GetID( ) ] = m_damageInterval;
        }
    }
}


void DamageOnCollide::DecrementDamageIntervalTimes( const float dt )
{
    // Container for removing times that have lasted full interval
    std::stack<int> idToRemove;

    // Decrement time intervals
    for ( auto timeInterval : m_damageTimeMap )
    {
        timeInterval.second -= dt;
        
        // move any times that have lasted full interval into
        //   container to be removed so damage may be applied again
        if ( timeInterval.second <= 0 )
        {
            idToRemove.push( timeInterval.first );
        }
    }

    // remove any intervals that have been on for full interval
    while ( !idToRemove.empty( ) )
    {
        auto id = idToRemove.top( );

        m_damageTimeMap.erase( id );

        idToRemove.pop( );
    }

}
