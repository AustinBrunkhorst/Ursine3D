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
#include <SweptControllerComponent.h>

NATIVE_COMPONENT_DEFINITION( DamageOnCollide );

using namespace ursine;
using namespace ecs;
using namespace resources;

DamageOnCollide::DamageOnCollide(void)
    : BaseComponent( )
    , m_damageToApply( 1.0f )
    , m_critModifier( 2.0f )
    , m_damageInterval( 0.5f )
    , m_deleteOnCollision( true )
    , m_deleted ( false )
    , m_spawnOnHit( false )
    , m_spawnOnDeath( false )
    , m_type( DAMAGE_ENEMY ) { }

DamageOnCollide::~DamageOnCollide(void)
{
    GetOwner( )->Listener(this)
        .Off( ENTITY_REMOVED, &DamageOnCollide::onDeath )
        .Off( ENTITY_COLLISION_PERSISTED, &DamageOnCollide::onCollide );

    m_damageTimeMap.clear( );
}

void DamageOnCollide::OnInitialize(void)
{
    GetOwner( )->Listener( this )
        .On( ENTITY_REMOVED, &DamageOnCollide::onDeath )
        .On( ENTITY_COLLISION_PERSISTED, &DamageOnCollide::onCollide );
}

DamageType DamageOnCollide::GetDamageType(void) const
{
    return m_type;
}

void DamageOnCollide::SetDamageType(DamageType type)
{
    m_type = type;
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

const ResourceReference &DamageOnCollide::GetArchetypeOnDeath(void) const
{
    return m_objToSpawn;
}

void DamageOnCollide::SetArchetypeOnDeath(const ResourceReference &objToSpawn)
{
    m_objToSpawn = objToSpawn;
}

const ResourceReference &DamageOnCollide::GetArchetypeOnHit(void) const
{
    return m_objToSpawnOnHit;
}

void DamageOnCollide::SetArchetypeOnHit(const ResourceReference &objToSpawn)
{
    m_objToSpawnOnHit = objToSpawn;
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

void DamageOnCollide::onCollide(EVENT_HANDLER(ursine::ecs::Entity))
{
    EVENT_ATTRS(Entity, physics::CollisionEventArgs);

    if (!m_deleted)
    {
        auto root = args->otherEntity->GetRoot( )->GetComponent<Health>( );
        auto entity = args->otherEntity->GetComponent<Health>( );

        if (!root && !entity)
            return;

        if (m_damageTimeMap.find( root->GetOwner( ) ) != m_damageTimeMap.end( ))
            return;

        // Make sure we can deal damage to this health type
        if (root && !root->CanDamage( this ))
            return;

        if (entity && !entity->CanDamage( this ))
            return;

        float damage = m_damageToApply;
        bool crit = false;

        // does object have crit
        if (args->otherEntity->HasComponent<CritSpot>( ))
        {
            crit = true;
            damage *= m_critModifier;

            auto critComp = args->otherEntity->GetComponent<CritSpot>( );

            // add other object to damage interval map
            //   if not deleting due to collision
            m_damageTimeMap[ critComp->GetOwner( ) ] = m_damageInterval;
            m_damageTimeMap[ critComp->GetOwner( )->GetRoot( ) ] = m_damageInterval;
        }

        if (root)
            applyDamage(root->GetOwner( ), args->contacts.front( ).point, damage, crit);
        else
            applyDamage(entity->GetOwner( ), args->contacts.front( ).point, damage, crit );

        spawnCollisionParticle(args->otherEntity, crit);
    }

    deleteOnCollision( );
}

void DamageOnCollide::applyDamage(const EntityHandle &obj, const SVec3& contact, float damage, bool crit)
{
    Health* healthComp = obj->GetComponent< Health >( );

    healthComp->DealDamage(contact, damage, crit );

    // add other object to damage interval map 
    //   if not deleting due to collision
    if (!m_deleteOnCollision)
    {
        m_damageTimeMap[ obj ] = m_damageInterval;
    }
}

void DamageOnCollide::DecrementDamageIntervalTimes(float dt)
{
    // Container for removing times that have lasted full interval
    std::stack< EntityHandle > idToRemove;

    // Decrement time intervals
    for (auto &timeInterval : m_damageTimeMap)
    {
        timeInterval.second -= dt;
        
        // move any times that have lasted full interval into
        //   container to be removed so damage may be applied again
        if (timeInterval.second <= 0.0f)
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

void DamageOnCollide::deleteOnCollision(void)
{
    // delete this projectile if delete on collision flag is set
    if (m_deleteOnCollision)
    {
        GetOwner( )->Delete( );
        m_deleted = true;
    }
}

void DamageOnCollide::onDeath(EVENT_HANDLER(ursine::ecs::Entity))
{
    if (m_spawnOnDeath)
    {
        auto obj = GetOwner( )->GetWorld( )
            ->CreateEntityFromArchetype( m_objToSpawn );

        if (obj)
            obj->GetTransform( )->SetWorldPosition(
                GetOwner( )->GetTransform( )->GetWorldPosition( )
            );
    }
}

void DamageOnCollide::getSpawnLocation(const EntityHandle &other, physics::RaycastOutput& rayout, ursine::SVec3& posToSet)
{
    EntityHandle entity;
    size_t size = rayout.entity.size( );

    for (size_t i = 0; i <size; ++i)
    {
        entity = GetOwner( )->GetWorld( )->GetEntity( rayout.entity[ i ] );

        if (entity == other)
        {
            posToSet = rayout.hit[ i ];

            break;
        }
    }
}

void DamageOnCollide::spawnCollisionParticle(const EntityHandle &other, bool crit)
{
    if (m_spawnOnHit)
    {
        // quick access to transform of owner
        Transform* trans = GetOwner( )->GetTransform( );

        // position for weapon to shoot to
        SVec3 pos = trans->GetWorldPosition( );

        if (crit)
        {
            // input for raycast check
            physics::RaycastInput rayin;
            
            // output from raycast check
            physics::RaycastOutput rayout;

            // for all objects be checked against
            Entity* entity = nullptr;

            SVec3 velocity;

            // need for path of bullet
            if (GetOwner( )->HasComponent<Rigidbody>( ))
                velocity = GetOwner( )->GetComponent<Rigidbody>( )->GetVelocity( );


            // is the owner of this component within the oject it is colliding with
            //   - if inside then we want to spawn particle behind the objects current position
            if (velocity.Dot(other->GetTransform( )->GetWorldPosition( ) - trans->GetWorldPosition( )) <= 0)
                velocity = -velocity;

            // raycast info
            rayin.start = trans->GetWorldPosition( );
            rayin.end = pos = rayin.start + velocity;

            // get ray to edge of other object
            GetOwner( )->GetWorld( )->GetEntitySystem<PhysicsSystem>( )->Raycast(rayin, rayout, physics::RAYCAST_ALL_HITS, false, 1.0f, false);

            getSpawnLocation(other->GetRoot( ), rayout, pos);
        }

        // create particle
        auto obj = GetOwner( )->GetWorld( )
            ->CreateEntityFromArchetype( m_objToSpawnOnHit );

        if (obj)
        {
            obj->GetTransform( )->SetWorldPosition( pos );

            // parent so that it follows objects and dies with object
            other->GetTransform( )->AddChild(obj->GetTransform( ));
        }
    }
}
