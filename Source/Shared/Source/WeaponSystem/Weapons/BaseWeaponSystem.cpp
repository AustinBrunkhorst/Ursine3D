/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BaseWeaponSystem.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include "Precompiled.h"
#include "BaseWeaponSystem.h"
#include "AbstractWeapon.h"
#include "AbstractHitScanWeapon.h"
#include "BaseWeaponComponent.h"
#include "HitscanWeaponComponent.h"
#include "WallComponent.h"
#include "DamageOnCollideComponent.h"
#include "ProjectileComponent.h"
#include "HealthComponent.h"
#include <RigidbodyComponent.h>
#include <TransformComponent.h>
#include <World.h>
#include <SystemConfig.h>
#include <Core/CoreSystem.h>
#include <SVec3.h>
#include <PhysicsSystem.h>
#include <AnimatorComponent.h>
#include "AudioEmitterComponent.h"
#include <ParticleEmitterComponent.h>
#include "TrailComponent.h"
#include "CritspotComponent.h"


ENTITY_SYSTEM_DEFINITION( BaseWeaponSystem );
ENTITY_SYSTEM_DEFINITION( HitscanWeaponSystem );

using namespace ursine;
using namespace ursine::ecs;

#define EXTRA_DIST 30.0f

namespace
{
    const std::string kFireGun = "FIRE_GUN_HAND";
    const std::string kTakeDamage = "PLAYER_TAKE_DAMAGE";
    const ursine::Randomizer random;

    // Apply spread / accuracy to shooting
    float GetSpreadValue(const float spread, const float accuracy)
    {
        return spread * (1.0f - accuracy);
    }

    // Give projectiles velocity with spread
    void ProjectileVelocity(AbstractWeapon &weapon, const EntityHandle &proj, Transform &trans)
    {
        float x_spread = GetSpreadValue( weapon.m_spread.GetValue( ), weapon.m_accuracy );
        float y_spread = GetSpreadValue( weapon.m_spread.GetValue( ), weapon.m_accuracy );

        // Get spray vecs
        ursine::SVec3 spray = trans.GetUp( ) * y_spread + trans.GetRight( ) * x_spread;

        // give projectile velocity and position
        proj->GetComponent<Rigidbody>( )->SetVelocity( trans.GetForward( ) * proj->GetComponent<Projectile>( )->GetSpeed( ) + spray );
    }

    void ProjectileSetUp(const EntityHandle &proj, AbstractWeapon &weapon)
    {
        // add projectile component if not found
        if (!proj->HasComponent<Projectile>( ))
        {
            proj->AddComponent<Projectile>( );
        }

        // add damage on collide component if not found
        if (!proj->HasComponent<DamageOnCollide>( ))
        {
            proj->AddComponent<DamageOnCollide>( );
        }

        // add rigidbody component if not found
        if (!proj->HasComponent<ursine::ecs::Rigidbody>( ))
        {
            proj->AddComponent<ursine::ecs::Rigidbody>( );
        }

        Projectile &projComp = *proj->GetComponent<Projectile>( );
        // set proj speed
        projComp.SetSpeed( weapon.GetProjSpeed( ) );
        // calc the lifet of proj base on weapons range
        projComp.CalculateLifeTime( weapon.GetMaxRange( ) );

        // set proj damage stats
        DamageOnCollide &damageComp = *proj->GetComponent<DamageOnCollide>( );
        damageComp.SetDamageToApply( weapon.GetDamageToApply( ) );
        damageComp.SetCritModifier( weapon.GetCritModifier( ) );
        damageComp.SetDamageInterval( weapon.GetDamageInterval( ) );
        damageComp.SetDeleteOnCollision( weapon.GetDeleteOnCollision( ) );

        proj->GetTransform( )->SetWorldPosition( weapon.m_firePosHandle->GetWorldPosition( ) );
    }

    // Is weapon out of ammo
    bool OutofAmmo(AbstractWeapon &weapon)
    {
        // can't reload if no ammo or clip is full
        if (weapon.m_ammoCount == 0)
            return true;

        return false;
    }

    // Is weapon's clip full
    bool ClipFull(AbstractWeapon &weapon)
    {
        return weapon.m_clipCount == weapon.m_clipSize;
    }

    // reload weapon
    int Reload(AbstractWeapon &weapon)
    {
        // Is weapon out of ammo?
        if (OutofAmmo( weapon ))
            return NO_AMMO;

        // is weapon's clip full
        if (ClipFull( weapon ))
            return CLIP_FULL;

        if (weapon.m_maxAmmoCount == UNLIMITED_AMMO)
            weapon.m_clipCount = weapon.m_clipSize;

        // reset clip to max
        else if (weapon.m_ammoCount > weapon.m_clipSize)
        {
            weapon.m_clipCount = weapon.m_clipSize;
            weapon.m_ammoCount -= weapon.m_clipSize;
        }

        // not enough ammo to reload fully
        else
        {
            weapon.m_clipCount = weapon.m_ammoCount;
            weapon.m_ammoCount = 0;
        }

        // set reload timer
        weapon.m_reloadTimer = weapon.m_reloadTime;

        return RELOAD_SUCCESS;
    }

    // Decrement time from weapon's fire timer
    void DecrementFireTimer(const float dt, AbstractWeapon &weapon)
    {
        if (!weapon.m_semiAutomatic)
            weapon.m_fireTimer -= dt;
    }

    // Decrement time from weapon's reload timer
    void DecrementReloadTimer(const float dt, AbstractWeapon &weapon)
    {
        weapon.m_reloadTimer -= dt;
    }

    // Tries to remove the number of rounds specified from the clip
    //   and returns the actual number of rounds removed
    int RemoveRoundsFromClip(AbstractWeapon &weapon)
    {
        // only shoot 5 if clip is unlimited
        if (weapon.m_clipSize == UNLIMITED_CLIP)
            return weapon.m_projFireCount;

        // how many rounds were removed
        int roundsRemoved = weapon.m_projFireCount;

        // remove rounds
        weapon.m_clipCount -= weapon.m_projFireCount;

        // if rounds removed were more than amount in clip
        if (weapon.m_clipCount < 0)
        {
            // calculating actual count removed
            roundsRemoved += weapon.m_clipCount;
            weapon.m_clipCount = 0;
        }

        return roundsRemoved;
    }

    void ReloadWeapon(AbstractWeapon &weapon)
    {
        URSINE_TODO("Have to apply reload animation");

        if (Reload( weapon )) { }
    }

    void DealHitscanDamage(const AbstractWeapon &weapon, Health &otherHealth)
    {
        otherHealth.DealDamage( weapon.GetDamageToApply( ) );
    }

    void ResetIdleSequence(AbstractWeapon *weapon)
    {
        // reset idle sequence
        /*weapon->m_animatorHandle->SetTimeScalar(1.0f);
        weapon->m_animatorHandle->SetAnimation("Gun_Idle");
        weapon->m_animatorHandle->SetPlaying(true);*/
    }
}


//////////////////////////////
////  Base Weapon System  ////
//////////////////////////////
BaseWeaponSystem::BaseWeaponSystem(World *world)
    : FilterSystem( world, Filter( ).One<BaseWeapon>( ).All<AudioEmitter>( ) ) {}

void BaseWeaponSystem::Enable(const EntityHandle &entity)
{
    auto uniqueID = entity->GetID( );

    // grab all comps needed
    if (entity->HasComponent<BaseWeapon>( ))
        m_weapons[ uniqueID ] = entity->GetComponent<BaseWeapon>( );

    m_transforms[ uniqueID ] = entity->GetTransform( );

    // grab audio emitter from root
    m_emitters[ uniqueID ] = entity->GetComponent<AudioEmitter>( );
}

void BaseWeaponSystem::Disable(const EntityHandle &entity)
{
    auto uniqueID = entity->GetID( );

    m_weapons.erase( uniqueID );
    m_transforms.erase( uniqueID );
    m_emitters.erase( uniqueID );
}

void BaseWeaponSystem::onUpdate(EVENT_HANDLER(World))
{
    float dt = Application::Instance->GetDeltaTime( );

    EvaluateProjectileWeapons( dt );
}

void BaseWeaponSystem::EvaluateProjectileWeapons(const float dt)
{
    for (auto it : m_weapons)
    {
        BaseWeapon *weapon = &*it.second;

        //weapon->m_animatorHandle->UpdateAnimation(dt);

        // skip if not active
        if (!weapon->m_active)
            continue;

        // Can weapon be fired
        switch (weapon->CanFire( ))
        {
        case RELOAD_IN_PROCESS:
            DecrementReloadTimer( dt, *weapon );
            break;
        case MUST_RELOAD:
            ReloadWeapon( *weapon );
            break;
        case FIRE_TIMER_SET:
            DecrementFireTimer( dt, *weapon );
            break;
        case TRIGGER_NOT_PULLED:
            ResetIdleSequence( weapon );
            break;
        case CAN_FIRE:
            FireProjectileWeapon( *weapon, it.first );
            break;
        default:
            ResetIdleSequence( weapon );
            break;
        }
    }
}

void BaseWeaponSystem::FireProjectileWeapon(BaseWeapon &weapon, ecs::EntityID id)
{
    if (weapon.FireLogic( ))
    {
        weapon.m_fireTimer = weapon.m_fireRate;

        // play sound
        //URSINE_TODO("Fix sound hack for weapons");
        // m_emitters[ id ]->AddSoundToPlayQueue(kFireGun);

        // reset firing sequence
        /*weapon.m_animatorHandle->SetAnimationTimePosition(0.1f);
        weapon.m_animatorHandle->SetTimeScalar(1.2f);
        weapon.m_animatorHandle->SetAnimation("Gun_Shoot");
        weapon.m_animatorHandle->SetPlaying(true);*/

        // number of rounds that were fired
        CreateProjectiles( weapon, *m_transforms[ id ], RemoveRoundsFromClip( weapon ) );
    }
}

void BaseWeaponSystem::CreateProjectiles(BaseWeapon &weapon, Transform &trans, const int projectilesFired)
{
    // Create the projectile that is desired to shoot
    auto proj = m_world->CreateEntityFromArchetype( 
        WORLD_ARCHETYPE_PATH + weapon.GetArchetypeToShoot( ), 
        "Bullet" 
    );

    // set up projectile stats based on gun
    ProjectileSetUp( proj, weapon );

    // temp vars for  creating projectiles
    EntityHandle cloneProj;
    ursine::SVec3 forwardVec = trans.GetForward( );

    // create the number of projectiles fired
    for (int i = 0; i < projectilesFired; ++i)
    {
        // create clone
        cloneProj = proj->Clone( );

        // give projectile a velocity
        ProjectileVelocity( weapon, cloneProj, trans );
    }
}


/////////////////////////////////
////  Hitscan Weapon System  ////
/////////////////////////////////

HitscanWeaponSystem::HitscanWeaponSystem(World *world)
    : FilterSystem( world, Filter( ).One<HitscanWeapon>( ).All<AudioEmitter>( ) ) { }

void HitscanWeaponSystem::Initialize(void)
{
    m_physicsSystem = m_world->GetEntitySystem<PhysicsSystem>( );
}

void HitscanWeaponSystem::Enable(const EntityHandle &entity)
{
    auto uniqueID = entity->GetID( );

    // grab all comps needed
    if (entity->HasComponent<HitscanWeapon>( ))
        m_weapons[ uniqueID ] = entity->GetComponent<HitscanWeapon>( );

    m_transforms[ uniqueID ] = entity->GetTransform( );

    // grab audio emitter from root
    m_emitters[ uniqueID ] = entity->GetComponent<AudioEmitter>( );
}

void HitscanWeaponSystem::Disable(const EntityHandle &entity)
{
    auto uniqueID = entity->GetID( );

    m_weapons.erase( uniqueID );
    m_transforms.erase( uniqueID );
    m_emitters.erase( uniqueID );
}

void HitscanWeaponSystem::onUpdate(EVENT_HANDLER(World))
{
    float dt = Application::Instance->GetDeltaTime( );

    EvaluateHitscanWeapons( dt );
}

void HitscanWeaponSystem::EvaluateHitscanWeapons(const float dt)
{
    for (auto it : m_weapons)
    {
        AbstractHitscanWeapon *weapon = it.second;

        //weapon->m_animatorHandle->UpdateAnimation( dt );

        if (!weapon->m_active)
            continue;

        // Can weapon be fired
        switch (weapon->CanFire( ))
        {
        case RELOAD_IN_PROCESS:
            DecrementReloadTimer( dt, *weapon );
            break;
        case MUST_RELOAD:
            ReloadWeapon( *weapon );
            break;
        case FIRE_TIMER_SET:
            DecrementFireTimer( dt, *weapon );
            break;
        case TRIGGER_NOT_PULLED:
            ResetIdleSequence( weapon );
            break;
        case CAN_FIRE:
            FireHitscanWeapon( *weapon, it.first );
            break;
        default:
            ResetIdleSequence( weapon );
            break;
        }
    }
}

void HitscanWeaponSystem::FireHitscanWeapon(AbstractHitscanWeapon &weapon, EntityID id)
{
    if (weapon.FireLogic( ))
    {
        weapon.m_fireTimer = weapon.m_fireRate;

        // play sound
        //m_emitters[ id ]->AddSoundToPlayQueue(kFireGun);

        //// reset firing sequence
        //weapon.m_animatorHandle->SetAnimationTimePosition(0.1f);
        //weapon.m_animatorHandle->SetTimeScalar(1.2f);
        //weapon.m_animatorHandle->SetAnimation("Gun_Shoot");
        //weapon.m_animatorHandle->SetPlaying(true);

        // create particle at weapons fire pos and parent to weapon
        auto e = m_world->CreateEntityFromArchetype( WORLD_ARCHETYPE_PATH + weapon.m_fireParticle );
        weapon.m_firePosHandle->AddChildAlreadyInLocal( e->GetTransform( ) );

        // number of rounds that were fired
        CreateRaycasts( weapon, *m_transforms[ id ], RemoveRoundsFromClip( weapon ) );
    }
}

void HitscanWeaponSystem::CreateRaycasts(AbstractHitscanWeapon &weapon, Transform &trans, const int projectilesFired)
{
    float x_spread; // x spread for proj
    float y_spread; // y spread for proj
    SVec3 spray; // spread vec to add to travel vecac
    SVec3 pos; // position for weapon to shoot to
    physics::RaycastInput rayin; // input for raycast check
    physics::RaycastOutput rayout; // output from raycast check

    for (int i = 0; i < projectilesFired; ++i)
    {
        x_spread = GetSpreadValue( weapon.m_spread.GetValue( ), weapon.m_accuracy );
        y_spread = GetSpreadValue( weapon.m_spread.GetValue( ), weapon.m_accuracy );

        // Get spray vecs
        spray = weapon.m_camHandle->GetUp( ) * y_spread + weapon.m_camHandle->GetRight( ) * x_spread;

        rayin.start = weapon.m_firePosHandle->GetWorldPosition( );
        rayin.end = weapon.m_camHandle->GetWorldPosition( ) + weapon.m_camHandle->GetForward( ) * (weapon.m_maxRange + EXTRA_DIST) + spray;

        if (m_physicsSystem->Raycast( rayin, rayout, weapon.m_raycastType, weapon.m_debug, weapon.m_drawDuration, weapon.m_alwaysDraw ))
        {
            switch (weapon.m_raycastType)
            {
                case physics::RAYCAST_ALL_HITS:
                    break;
                case physics::RAYCAST_CLOSEST_HIT:
                {
                    auto delta = rayin.end - rayin.start;

                    RaycastClosestHitLogic( delta, rayout, weapon );
                    break;
                }
                default:
                    break;
            }
        }
        else
            CreateTrail( weapon, rayin.end );
    }
}

void HitscanWeaponSystem::RaycastClosestHitLogic(ursine::SVec3 &raycastVec, ursine::physics::RaycastOutput &rayout, AbstractHitscanWeapon &weapon)
{
    EntityHandle e;

    // get first object hit w/ health and apply damage
    auto objHit = m_world->GetEntity( rayout.entity.front( ) );

    // where did rayact collide at
    ursine::SVec3 &collisionPoint = rayout.hit[ 0 ];

    // create shot particle
    e = m_world->CreateEntityFromArchetype( WORLD_ARCHETYPE_PATH + weapon.m_shotParticle );
    e->GetTransform( )->SetWorldPosition( collisionPoint );

    // was a crit spot hit
    if (objHit->HasComponent<CritSpot>( ))
    {
        // apply crit damage
        objHit->GetComponent<CritSpot>( )->ApplyDamage( weapon.m_damageToApply * weapon.m_critModifier );

        // find actual hit position on obj hit
        SpawnCollisionParticle( collisionPoint, raycastVec, objHit );
        e->GetTransform( )->SetWorldPosition( collisionPoint );

        // parent so that it follows objects and dies with object
        objHit->GetTransform( )->AddChild( e->GetTransform( ) );
    }

    // if object has health
    else if (objHit->HasComponent<Health>( ))
    {
        DealHitscanDamage( weapon, *objHit->GetComponent<Health>( ) );

        objHit->GetTransform( )->AddChild( e->GetTransform( ) );
    }


    CreateTrail( weapon, collisionPoint );
}


void HitscanWeaponSystem::CreateTrail(AbstractHitscanWeapon &weapon, ursine::SVec3 &trailEnd)
{
    // create trial for raycast
    auto e = m_world->CreateEntityFromArchetype( WORLD_ARCHETYPE_PATH + weapon.m_trailParticle );
    e->GetTransform( )->SetWorldPosition( weapon.m_firePosHandle->GetWorldPosition( ) );

    // check if trail comp was present
    //   done after spawning of other particles because endpoint may change if crit spot was hit
    if (e->HasComponent<TrailComponent>( ))
    {
        e->GetComponent<TrailComponent>( )->SetVecToEnd( trailEnd );
    }
}


void HitscanWeaponSystem::GetSpawnLocation(const EntityHandle &other, ursine::physics::RaycastOutput &rayout, ursine::SVec3 &posToSet)
{
    EntityHandle entity;
    size_t size = rayout.entity.size( );

    for (size_t i = 0; i < size; ++i)
    {
        entity = m_world->GetEntity( rayout.entity[ i ] );

        if (entity == other)
        {
            posToSet = rayout.hit[ i ];
            break;
        }
    }
}


void HitscanWeaponSystem::SpawnCollisionParticle(ursine::SVec3 &collisionPoint, ursine::SVec3 &raycastVec, const EntityHandle &other)
{
    ursine::physics::RaycastInput rayin; // input for raycast check
    ursine::physics::RaycastOutput rayout; // output from raycast check

    raycastVec.Normalize( );

    // raycast info
    rayin.start = collisionPoint;
    rayin.end = rayin.start + (EXTRA_DIST * raycastVec);

    // get ray to edge of other object
    m_physicsSystem->Raycast( rayin, rayout, ursine::physics::RAYCAST_ALL_HITS, false, 1.0f, false );

    GetSpawnLocation( other->GetRoot( ), rayout, collisionPoint );
}
