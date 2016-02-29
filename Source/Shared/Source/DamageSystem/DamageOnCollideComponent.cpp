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

using namespace ursine;
using namespace ecs;

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

    if (!m_deleted)
    {
        auto root = args->otherEntity->GetRoot( );

        if (!root->HasComponent<Health>( ))
            return;

        if (m_damageTimeMap.find( root->GetUniqueID( ) ) != m_damageTimeMap.end( ))
            return;

        float damage = m_damageToApply;
        bool crit = false;

        // does object have crit
        if ( args->otherEntity->HasComponent< CritSpot >( ) )
        {
            crit = true;
            damage *= m_critModifier;
        }

        applyDamage(root, args->contacts.front( ).point, damage, crit);

        spawnCollisionParticle(args->otherEntity, crit);
    }

    deleteOnCollision( );
}

void DamageOnCollide::applyDamage(ursine::ecs::Entity* obj, const ursine::SVec3& contact, float damage, bool crit)
{   
    Health* healthComp = obj->GetComponent< Health >( );

    healthComp->DealDamage(contact, damage, crit );

    // add other object to damage interval map 
    //   if not deleting due to collision
    if (!m_deleteOnCollision)
    {
        m_damageTimeMap[ obj->GetUniqueID( ) ] = m_damageInterval;
    }
}

#include <iostream>
void DamageOnCollide::DecrementDamageIntervalTimes(float dt)
{
    // Container for removing times that have lasted full interval
    std::stack< int > idToRemove;

    // Decrement time intervals
    for (auto &timeInterval : m_damageTimeMap)
    {
        timeInterval.second -= dt;
        
        std::cout << "In HEre" << std::endl;
        // move any times that have lasted full interval into
        //   container to be removed so damage may be applied again
        if (timeInterval.second <= 0.0f)
        {
            std::cout << "HERE" << std::endl;
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


void DamageOnCollide::onDeath(EVENT_HANDLER(ursine::ecs::ENTITY_REMOVED))
{
    if (m_spawnOnDeath && m_objToSpawn != ".uatype")
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


void DamageOnCollide::spawnCollisionParticle(ursine::ecs::Entity* other, bool crit)
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
            if ( velocity.Dot(other->GetTransform( )->GetWorldPosition( ) - trans->GetWorldPosition( )) <= 0 )
                velocity = -velocity;

            // raycast info
            rayin.start = trans->GetWorldPosition( );
            rayin.end = pos = rayin.start + velocity;

            // get ray to edge of other object
            GetOwner( )->GetWorld( )->GetEntitySystem<PhysicsSystem>( )->Raycast(rayin, rayout, physics::RAYCAST_ALL_HITS, false, 1.0f, false);

            getSpawnLocation(other->GetRoot( ), rayout, pos);
        }

        if (m_objToSpawnOnHit != ".uatype")
        {
            // create particle
            ursine::ecs::Entity* obj = GetOwner( )->GetWorld( )->CreateEntityFromArchetype(WORLD_ARCHETYPE_PATH + m_objToSpawnOnHit);
            obj->GetTransform( )->SetWorldPosition( pos );

            // parent so that it follows objects and dies with object
            other->GetTransform( )->AddChild(obj->GetTransform( ));
        }
    }
}

