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
#include "CritSpotComponent.h"
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
    GetOwner( )->Listener(this)
        .Off( ursine::ecs::ENTITY_COLLISION_PERSISTED, &DamageOnCollide::OnCollide );

    m_damageTimeMap.clear( );
}

void DamageOnCollide::OnInitialize( void )
{
    GetOwner( )->Listener( this )
        .On( ursine::ecs::ENTITY_COLLISION_PERSISTED, &DamageOnCollide::OnCollide);
}

float DamageOnCollide::GetDamageToApply( void ) const
{
    if ( m_deleted )
        return 0.0f;

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

void DamageOnCollide::OnCollide( EVENT_HANDLER( ursine::ecs::ENTITY_COLLISION_PERSISTED ) )
{
    EVENT_ATTRS(ursine::ecs::Entity, ursine::physics::CollisionEventArgs);

    if ( args->otherEntity->HasComponent<CritSpot>( ) )
        return;

    // Try and grab health Component
    Health* healthComp = args->otherEntity->GetComponent<Health>( );

    // make sure that health comp was found and that we have not damaged obj recently
    if ( healthComp && m_damageTimeMap[ healthComp->GetOwner( )->GetUniqueID( ) ] == NULL )
    {
        ApplyDamage( healthComp );
    }

}

void DamageOnCollide::ApplyDamage(Health* healthComp)
{
    // apply damage to object
    healthComp->DealDamage(m_damageToApply);

    // add other object to damage interval map 
    //   if not deleting due to collision
    if ( !DeleteOnCollision( ) )
    {
        m_damageTimeMap[ healthComp->GetOwner( )->GetUniqueID( ) ] = m_damageInterval;
    }
}


void DamageOnCollide::AddEntityToIntervals(ursine::ecs::EntityUniqueID uniqueID)
{
    if ( !DeleteOnCollision( ) )
    {
        m_damageTimeMap[ uniqueID ] = m_damageInterval;
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


bool DamageOnCollide::DeleteOnCollision(void)
{
    // delete this projectile if delete on collision flag is set
    if ( m_deleteOnCollision )
    {
        GetOwner( )->Delete( );
        m_deleted = true;
    }

    return m_deleteOnCollision;
}
