/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
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
#include <SVec3.h>
#include <PhysicsSystem.h>

NATIVE_COMPONENT_DEFINITION( DamageOnCollide );

DamageOnCollide::DamageOnCollide( void )
    : BaseComponent( )
    , m_damageToApply( 1.0f )
    , m_critModifier( 2.0f )
    , m_damageInterval( 0.5f )
    , m_objToSpawn("")
    , m_deleteOnCollision( true )
    , m_deleted ( false )
    , m_spawnOnHit( false )
    , m_spawnOnDeath( false )
{
}

DamageOnCollide::~DamageOnCollide(void)
{
    GetOwner( )->Listener(this)
        .Off( ursine::ecs::ENTITY_REMOVED, &DamageOnCollide::onDeath )
        .Off( ursine::ecs::ENTITY_COLLISION_PERSISTED, &DamageOnCollide::onCollide );

    m_damageTimeMap.clear( );
}

void DamageOnCollide::OnInitialize(void)
{
    GetOwner( )->Listener( this )
        .On( ursine::ecs::ENTITY_REMOVED, &DamageOnCollide::onDeath )
        .On( ursine::ecs::ENTITY_COLLISION_PERSISTED, &DamageOnCollide::onCollide );
}

float DamageOnCollide::GetDamageToApply(void) const
{
    if (m_deleted)
        return 0.0f;

    return m_damageToApply;
}

void DamageOnCollide::SetDamageToApply(float damage)
{
    m_damageToApply = damage;
}

float DamageOnCollide::GetCritModifier(void) const
{
    return m_critModifier;
}

void DamageOnCollide::SetCritModifier(float modifier)
{
    m_critModifier = modifier;
}

float DamageOnCollide::GetDamageInterval(void) const
{
    return m_damageInterval;
}

void DamageOnCollide::SetDamageInterval(float damageInterval)
{
    m_damageInterval = damageInterval;
}

const std::string &DamageOnCollide::GetArchetypeOnDeath(void) const
{
    return m_objToSpawn;
}

void DamageOnCollide::SetArchetypeOnDeath(const std::string &objToSpawn)
{
    m_objToSpawn = objToSpawn;

    if (m_objToSpawn.find(".uatype") == std::string::npos)
        m_objToSpawn += ".uatype";
}

const std::string &DamageOnCollide::GetArchetypeOnHit(void) const
{
    return m_objToSpawnOnHit;
}

void DamageOnCollide::SetArchetypeOnHit(const std::string &objToSpawn)
{
    m_objToSpawnOnHit = objToSpawn;

    if (m_objToSpawnOnHit.find(".uatype") == std::string::npos)
        m_objToSpawnOnHit += ".uatype";
}


bool DamageOnCollide::GetDeleteOnCollision(void) const
{
    return m_deleteOnCollision;
}

void DamageOnCollide::SetDeleteOnCollision(bool state)
{
    m_deleteOnCollision = state;
}


bool DamageOnCollide::GetSpawnOnDeath(void) const
{
    return m_spawnOnDeath;
}
void DamageOnCollide::SetSpawnOnDeath(bool state)
{
    m_spawnOnDeath = state;
}

bool DamageOnCollide::GetSpawnOnHit(void) const
{
    return m_spawnOnHit;
}
void DamageOnCollide::SetSpawnOnHit(bool state)
{
    m_spawnOnHit = state;
}

void DamageOnCollide::onCollide(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED))
{
    EVENT_ATTRS(ursine::ecs::Entity, ursine::physics::CollisionEventArgs);

    if (args->otherEntity->HasComponent< CritSpot >( ))
    {
        applyCritDamage( args->otherEntity->GetComponent< CritSpot >( ), args->contacts );
    }

    // make sure that health comp was found and that we have not damaged obj recently
    else if (args->otherEntity->HasComponent< Health >( ))
    {
        applyDamage( args->otherEntity->GetComponent< Health >( ), args->contacts );
    }

}

void DamageOnCollide::applyCritDamage(CritSpot *critComp, ContactsArg contacts)
{
    if (m_damageTimeMap[ critComp->GetOwner( )->GetRoot( )->GetUniqueID( ) ] != NULL)
        return;

    float damage = m_damageToApply * m_critModifier;
    auto root = critComp->GetOwner( )->GetRoot( );

    critComp->ApplyDamage( damage );

    spawnCollisionParticle( critComp->GetOwner( ) );

    sendDamageEvent( { damage, true, root, contacts[ 0 ].point } );
    
    // add other object to damage interval map 
    //   if not deleting due to collision
    if (!deleteOnCollision( ))
    {
        m_damageTimeMap[ critComp->GetOwner( )->GetUniqueID( ) ] = m_damageInterval;
        m_damageTimeMap[ critComp->GetOwner( )->GetRoot( )->GetUniqueID( ) ] = m_damageInterval;
    }
}


void DamageOnCollide::applyDamage(Health *healthComp, ContactsArg contacts)
{
    if (m_damageTimeMap[ healthComp->GetOwner( )->GetUniqueID( ) ] != NULL)
        return;

    // apply damage to object
    healthComp->DealDamage( m_damageToApply );

    spawnCollisionParticle( healthComp->GetOwner( ) );

    sendDamageEvent( { m_damageToApply, false, healthComp->GetOwner( ), contacts[ 0 ].point } );

    // add other object to damage interval map 
    //   if not deleting due to collision
    if (!deleteOnCollision( ))
    {
        m_damageTimeMap[ healthComp->GetOwner( )->GetUniqueID( ) ] = m_damageInterval;
    }
}

void DamageOnCollide::DecrementDamageIntervalTimes(float dt)
{
    // Container for removing times that have lasted full interval
    std::stack< int > idToRemove;

    // Decrement time intervals
    for (auto timeInterval : m_damageTimeMap)
    {
        timeInterval.second -= dt;
        
        // move any times that have lasted full interval into
        //   container to be removed so damage may be applied again
        if (timeInterval.second <= 0)
        {
            idToRemove.push( timeInterval.first );
        }
    }

    // remove any intervals that have been on for full interval
    while (!idToRemove.empty( ))
    {
        auto id = idToRemove.top( );

        m_damageTimeMap.erase( id );

        idToRemove.pop( );
    }

}


bool DamageOnCollide::deleteOnCollision(void)
{
    // delete this projectile if delete on collision flag is set
    if (m_deleteOnCollision)
    {
        GetOwner( )->Delete( );
        m_deleted = true;
    }

    return m_deleteOnCollision;
}


void DamageOnCollide::onDeath(EVENT_HANDLER(ursine::ecs::ENTITY_REMOVED))
{
    if (m_spawnOnDeath)
    {
        ursine::ecs::Entity* obj = GetOwner( )->GetWorld( )->CreateEntityFromArchetype(WORLD_ARCHETYPE_PATH + m_objToSpawn);

        obj->GetTransform( )->SetWorldPosition(GetOwner( )->GetTransform( )->GetWorldPosition( ));
    }
}


void DamageOnCollide::getSpawnLocation(ursine::ecs::Entity* other, ursine::physics::RaycastOutput& rayout, ursine::SVec3& posToSet)
{
    ursine::ecs::Entity* entity;
    size_t size = rayout.entity.size( );

    for (size_t i = 0; i <size; ++i)
    {
        entity = GetOwner( )->GetWorld( )->GetEntityUnique( rayout.entity[ i ] );

        if (entity == other)
        {
            posToSet = rayout.hit[ i ];
            break;
        }
    }
}


void DamageOnCollide::spawnCollisionParticle(ursine::ecs::Entity* other)
{
    //if (m_spawnOnHit)
    //{
    //    ursine::physics::RaycastInput rayin;   // input for raycast check
    //    ursine::physics::RaycastOutput rayout; // output from raycast check
    //
    //    ursine::ecs::Transform* trans = GetOwner( )->GetTransform( );      // quick access to transform of owner
    //    ursine::SVec3 pos = trans->GetWorldPosition( ); // position for weapon to shoot to
    //
    //    ursine::ecs::Entity* entity = nullptr; // for all objects be checked against
    //
    //    ursine::SVec3 velocity = GetOwner( )->GetComponent<ursine::ecs::Rigidbody>( )->GetVelocity( ); // need for path of bullet
    //
    //
    //    // is the owner of this component within the oject it is colliding with
    //    //   - if inside then we want to spawn particle behind the objects current position
    //    if (velocity.Dot( other->GetTransform( )->GetWorldPosition( ) - trans->GetWorldPosition( ) ) <= 0)
    //        velocity = -velocity;
    //
    //    // raycast info
    //    rayin.start = trans->GetWorldPosition( );
    //    rayin.end = pos = rayin.start + velocity;
    //
    //    // get ray to edge of other object
    //    GetOwner( )->GetWorld( )->GetEntitySystem<ursine::ecs::PhysicsSystem>( )->Raycast( rayin, rayout, ursine::physics::RAYCAST_ALL_HITS, false, 1.0f, false );
    //
    //    getSpawnLocation(other->GetRoot( ), rayout, pos);
    //
    //    // create particle
    //    ursine::ecs::Entity* obj = GetOwner( )->GetWorld( )->CreateEntityFromArchetype( WORLD_ARCHETYPE_PATH + m_objToSpawn );
    //    obj->GetTransform( )->SetWorldPosition( pos );
    //
    //    // parent so that it follows objects and dies with object
    //    other->GetTransform( )->AddChild( obj->GetTransform( ) );
    //}
}

void DamageOnCollide::sendDamageEvent(DamageEventArgs args)
{
    GetOwner( )->GetWorld( )->Dispatch( WORLD_DAMAGE_EVENT, &args );
}

