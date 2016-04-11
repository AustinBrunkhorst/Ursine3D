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
#include "AbstractProjWeapon.h"
#include "AbstractHitScanWeapon.h"
#include "BaseWeaponComponent.h"
#include "HitscanWeaponComponent.h"
#include "HealthComponent.h"
#include <TransformComponent.h>
#include <World.h>
#include <SystemConfig.h>
#include <Core/CoreSystem.h>
#include <SVec3.h>
#include <PhysicsSystem.h>
#include "AudioEmitterComponent.h"
#include "TrailComponent.h"
#include "CritspotComponent.h"
#include <Core/Audio/AudioManager.h>
#include "GameEvents.h"
#include "GhostComponent.h"
#include "AIHordelingTypeComponent.h"

ENTITY_SYSTEM_DEFINITION( BaseWeaponSystem );
ENTITY_SYSTEM_DEFINITION( HitscanWeaponSystem );

using namespace ursine;
using namespace ecs;

#define EXTRA_DIST 30.0f

namespace
{
    const Randomizer random;

    size_t FindNonGhost(EntityHandle& entityToSet, physics::RaycastOutput& rayout, World* world)
    {

        for ( size_t i = rayout.entity.size( ) - 1; i >= 0; --i )
        {
            entityToSet = world->GetEntity( rayout.entity[ i ] );

            if ( !entityToSet->HasComponent< Ghost >( ) || entityToSet->GetRoot( )->HasComponent< AIHordelingType >( ) )
                return i;
        }

        return -1;
    }

    int Test(int j)
    {
        int i = 0;

        j += 5;

        i += 10;

        return j * i;
    }
}

// Apply spread / accuracy to shooting
void WeaponSystemUtils::GetSpreadValues(Randomizer& spread, float accuracy, float& xSpread, float& ySpread)
{
    xSpread = spread.GetValue( ) * ( 1.0f - accuracy );
    ySpread = spread.GetValue( ) * ( 1.0f - accuracy );
}

void WeaponSystemUtils::ConstructRaycast(AbstractWeapon& weapon, SVec3& start, SVec3& end)
{
    float x_spread, y_spread;

    GetSpreadValues( weapon.m_spread, weapon.m_accuracy, x_spread, y_spread );

    // Get spray vecs
    SVec3 spray = weapon.m_camHandle->GetUp( ) * y_spread + weapon.m_camHandle->GetRight( ) * x_spread;

    start = weapon.m_firePosHandle->GetWorldPosition( );
    end = weapon.m_camHandle->GetWorldPosition( ) + weapon.m_camHandle->GetForward( ) * ( weapon.m_maxRange + EXTRA_DIST ) + spray;
}

// Give projectiles velocity with spread
void WeaponSystemUtils::ProjectileVelocity(AbstractProjWeapon& weapon, EntityHandle& proj)
{
    SVec3 start, end;  

    ConstructRaycast(weapon, start, end);

    start = end - start;
    start.Normalize( );

    // give projectile velocity and position
    game::ProjectileInitEventArgs args(start, weapon.m_maxRange);

    proj->Dispatch( game::PROJECTILE_INIT, &args );
}

void WeaponSystemUtils::ProjectileSetUp(EntityHandle& proj, AbstractProjWeapon& weapon)
{
    proj->GetTransform( )->SetWorldPosition( weapon.m_firePosHandle->GetWorldPosition( ) );


    ProjectileVelocity( weapon, proj );
}

// Is weapon out of ammo
bool WeaponSystemUtils::OutofAmmo(AbstractWeapon &weapon)
{
    // can't reload if no ammo or clip is full
    if (weapon.m_ammoCount == 0)
        return true;

    return false;
}

// Is weapon's clip full
bool WeaponSystemUtils::ClipFull(AbstractWeapon &weapon)
{
    return weapon.m_clipCount == weapon.m_clipSize;
}

// reload weapon
int WeaponSystemUtils::Reload(AbstractWeapon &weapon)
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
void WeaponSystemUtils::DecrementFireTimer(float dt, AbstractWeapon &weapon)
{
    if (!weapon.m_semiAutomatic)
        weapon.m_fireTimer -= dt;
}

// Decrement time from weapon's reload timer
void WeaponSystemUtils::DecrementReloadTimer(float dt, AbstractWeapon &weapon)
{
    weapon.m_reloadTimer -= dt;

    if (weapon.m_reloadTimer <= 0.0f)
        weapon.m_owner->GetRoot( )->Dispatch( game::RELOAD_END, EventArgs::Empty );
}

// Tries to remove the number of rounds specified from the clip
//   and returns the actual number of rounds removed
int WeaponSystemUtils::RemoveRoundsFromClip(AbstractWeapon &weapon)
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

void WeaponSystemUtils::ReloadWeapon(AbstractWeapon &weapon, ursine::ecs::AudioEmitter *emitter)
{
    if (Reload( weapon ))
    {
        // play sound
        emitter->PushEvent( weapon.GetReloadSFX( ) );
        weapon.m_owner->GetRoot( )->Dispatch( game::RELOAD_START, EventArgs::Empty );
    }
}

void WeaponSystemUtils::ResetIdleSequence(AbstractWeapon* weapon)
{
    // reset idle sequence
    /*weapon->m_animatorHandle->SetTimeScalar(1.0f);
    weapon->m_animatorHandle->SetAnimation("Gun_Idle");
    weapon->m_animatorHandle->SetPlaying(true);*/
}

//////////////////////////////
////  Base Weapon System  ////
//////////////////////////////

BaseWeaponSystem::BaseWeaponSystem(World *world) 
    : FilterSystem( world, Filter( ).One< BaseWeapon >( ) )
{ }

void BaseWeaponSystem::Enable(const EntityHandle &entity)
{
    // grab all comps needed
    if (entity->HasComponent< BaseWeapon >( ))
        m_weapons[ entity ] = entity->GetComponent< BaseWeapon >( );

    m_transforms[ entity ] = entity->GetTransform( );
 
    // grab audio emitter from root
    m_emitters[ entity ] = entity->GetComponentInParent< AudioEmitter >( );
}

void BaseWeaponSystem::Disable(const EntityHandle &entity)
{
    m_weapons.erase( entity );
    m_transforms.erase( entity );
    m_emitters.erase( entity );
}

void BaseWeaponSystem::onUpdate(EVENT_HANDLER(World))
{
    float dt = Application::Instance->GetDeltaTime( );

    EvaluateProjectileWeapons( dt );
}

void BaseWeaponSystem::EvaluateProjectileWeapons(float dt)
{
    for (auto it : m_weapons)
    {
        AbstractProjWeapon* weapon = it.second;

        // skip if not active
        if (!weapon->m_active)
            continue;

        // Can weapon be fired
        switch (weapon->CanFire( ))
        {
        case RELOAD_IN_PROCESS:
            WeaponSystemUtils::DecrementReloadTimer( dt, *weapon );
            break;
        case MUST_RELOAD:
            WeaponSystemUtils::ReloadWeapon( *weapon, m_emitters[ it.first ] );
            break;
        case FIRE_TIMER_SET:
            WeaponSystemUtils::DecrementFireTimer( dt, *weapon );
            break;
        case TRIGGER_NOT_PULLED:
            WeaponSystemUtils::ResetIdleSequence( weapon );
            break;
        case CAN_FIRE:
            FireProjectileWeapon( *weapon, it.first );
            break;
        default:
            WeaponSystemUtils::ResetIdleSequence( weapon );
            break;
        }
    }
}

void BaseWeaponSystem::FireProjectileWeapon(AbstractProjWeapon& weapon, const EntityHandle &entity)
{
    if (weapon.FireLogic( ))
    {
        weapon.m_fireTimer = weapon.m_fireRate;

        // play sound
        AudioEmitter* emitter = m_emitters[ entity ];

        if ( emitter )
            emitter->PushEvent( weapon.GetShootSFX( ) );

        // reset firing sequence
        /*weapon.m_animatorHandle->SetAnimationTimePosition(0.1f);
        weapon.m_animatorHandle->SetTimeScalar(1.2f);
        weapon.m_animatorHandle->SetAnimation("Gun_Shoot");
        weapon.m_animatorHandle->SetPlaying(true);*/

        // create particle at weapons fire pos and parent to weapon
        auto e = m_world->CreateEntityFromArchetype( weapon.m_fireParticle );

        if (e)
        weapon.m_firePosHandle->AddChildAlreadyInLocal(e->GetTransform( ));


        // number of rounds that were fired
        CreateProjectiles(
            weapon, *m_transforms[ entity ], 
            WeaponSystemUtils::RemoveRoundsFromClip( weapon ) 
        );
    }
}

void BaseWeaponSystem::CreateProjectiles(AbstractProjWeapon& weapon, Transform& trans, int projectilesFired)
{
    // make sure projectile was fired
    if ( !projectilesFired )
        return;

    // Create the projectile that is desired to shoot
    auto proj = m_world->CreateEntityFromArchetype( 
        weapon.GetArchetypeToShoot( )
    );

    UAssert(proj, "No archtype was set to be shot by weapon.");

    // set up projectile stats based on gun
    WeaponSystemUtils::ProjectileSetUp( proj, weapon );

    // temp vars for  creating projectiles
    EntityHandle cloneProj = nullptr;

    // create the number of projectiles fired
    for ( int i = 1; i < projectilesFired; ++i )
    {
        // create clone
        cloneProj = proj->Clone( );

        // give projectile a velocity
        WeaponSystemUtils::ProjectileVelocity( weapon, cloneProj );
    }
}

/////////////////////////////////
////  Hitscan Weapon System  ////
/////////////////////////////////

HitscanWeaponSystem::HitscanWeaponSystem(World *world)
    : FilterSystem( world, Filter( ).One<HitscanWeapon>( ) ) 
{ }

void HitscanWeaponSystem::Initialize(void)
{
    m_physicsSystem = m_world->GetEntitySystem<PhysicsSystem>( );
}

void HitscanWeaponSystem::Enable(const EntityHandle &entity)
{
    // grab all comps needed
    if (entity->HasComponent<HitscanWeapon>( ))
        m_weapons[ entity ] = entity->GetComponent<HitscanWeapon>( );

    m_transforms[ entity ] = entity->GetTransform( );

    // grab audio emitter from root
    m_emitters[ entity ] = entity->GetComponentInParent<AudioEmitter>( );
}

void HitscanWeaponSystem::Disable(const EntityHandle &entity)
{
    m_weapons.erase( entity );
    m_transforms.erase( entity );
    m_emitters.erase( entity );
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
            WeaponSystemUtils::DecrementReloadTimer( dt, *weapon );
            break;
        case MUST_RELOAD:
            WeaponSystemUtils::ReloadWeapon( *weapon, m_emitters[ it.first ] );
            break;
        case FIRE_TIMER_SET:
            WeaponSystemUtils::DecrementFireTimer( dt, *weapon );
            break;
        case TRIGGER_NOT_PULLED:
            WeaponSystemUtils::ResetIdleSequence( weapon );
            break;
        case CAN_FIRE:
            FireHitscanWeapon( *weapon, it.first );
            break;
        default:
            WeaponSystemUtils::ResetIdleSequence( weapon );
            break;
        }
    }
}

void HitscanWeaponSystem::FireHitscanWeapon(AbstractHitscanWeapon &weapon, const EntityHandle &entity)
{
    if (weapon.FireLogic( ))
    {
        weapon.m_fireTimer = weapon.m_fireRate;

        // play sound
        AudioEmitter* emitter = m_emitters[ entity ];

        if ( emitter )
            emitter->PushEvent( weapon.GetShootSFX( ) );

        //// reset firing sequence
        //weapon.m_animatorHandle->SetAnimationTimePosition(0.1f);
        //weapon.m_animatorHandle->SetTimeScalar(1.2f);
        //weapon.m_animatorHandle->SetAnimation("Gun_Shoot");
        //weapon.m_animatorHandle->SetPlaying(true);

        // create particle at weapons fire pos and parent to weapon
        auto e = m_world->CreateEntityFromArchetype(weapon.m_fireParticle);

        if (e)
        {
            e->GetTransform( )->SetWorldPosition( SVec3( 0.0f, 0.0f, 0.0f ) );
            weapon.m_firePosHandle->AddChildAlreadyInLocal(e->GetTransform( ));
        }
            
        
        // number of rounds that were fired
        CreateRaycasts(
            weapon, *m_transforms[ entity ], 
            WeaponSystemUtils::RemoveRoundsFromClip( weapon ) 
        );
    }
}

void HitscanWeaponSystem::CreateRaycasts(AbstractHitscanWeapon &weapon, Transform &trans, const int projectilesFired)
{
    physics::RaycastInput rayin;   // input for raycast check
    physics::RaycastOutput rayout; // output from raycast check

    int j = 0;

    for ( int i = 0; i < projectilesFired; ++i )
    {
        WeaponSystemUtils::ConstructRaycast( weapon, rayin.start, rayin.end );

        if (m_physicsSystem->Raycast( rayin, rayout, physics::RAYCAST_CLOSEST_NON_GHOST, weapon.m_debug, weapon.m_drawDuration, Color(1.0f, 0.0f, 0.0f, 1.0f), weapon.m_alwaysDraw ))
        {
            switch (weapon.m_raycastType)
            {
            case physics::RAYCAST_ALL_HITS:
                break;
            case physics::RAYCAST_CLOSEST_HIT:
            {
                    auto delta = rayin.end - rayin.start;

                    if ( !RaycastClosestHitLogic( delta, rayout, weapon ) )
                        CreateTrail(weapon, rayin.end);
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

bool HitscanWeaponSystem::RaycastClosestHitLogic(SVec3 &raycastVec, physics::RaycastOutput &rayout, AbstractHitscanWeapon &weapon)
{
    EntityHandle e;

    EntityHandle objHit = m_world->GetEntity( rayout.entity[ 0 ] );

    // where did rayact collide at
    SVec3 &collisionPoint = rayout.hit[ 0 ];

    // create shot particle
    e = m_world->CreateEntityFromArchetype( weapon.m_shotParticle );

    float damage = weapon.m_damageToApply;
    bool crit = false;

    // was a crit spot hit
    if (objHit->HasComponent<CritSpot>( ))
    {
        crit = true;

        // apply crit modifier to damage
        damage *= weapon.m_critModifier;

        // find actual hit position on obj hit
        if ( objHit->GetComponent< CritSpot >( )->GetSearch( ) )
            SpawnCollisionParticle( collisionPoint, raycastVec, objHit );
    }

    if ( e )
        e->GetTransform( )->SetWorldPosition(collisionPoint);

    // if object has health
    if ( objHit->GetRoot( )->HasComponent<Health>( ) )
    {
        Health* rootHealth = objHit->GetRoot( )->GetComponent< Health >( );

        if ( rootHealth->CanDamage(&weapon) )
            rootHealth->DealDamage(collisionPoint, damage, crit);

        else
            return false;
    }
    else if ( objHit->GetComponent<Health>( ) )
    {
        Health* objHealth = objHit->GetComponent< Health >( );

        if ( objHealth->CanDamage(&weapon) )
            objHealth->DealDamage(collisionPoint, damage, crit);

        else
            return false;
    }

    if ( !crit && e )
        objHit->GetTransform( )->AddChild(e->GetTransform( ));

    CreateTrail( weapon, collisionPoint );

    return true;
}

void HitscanWeaponSystem::CreateTrail(AbstractHitscanWeapon &weapon, SVec3 &trailEnd)
{
    // create trial for raycast
    auto e = m_world->CreateEntityFromArchetype( weapon.m_trailParticle );

    if (!e)
        return;

    e->GetTransform( )->SetWorldPosition( weapon.m_firePosHandle->GetWorldPosition( ) );

    // check if trail comp was present
    //   done after spawning of other particles because endpoint may change if crit spot was hit
    if (e->HasComponent<TrailComponent>( ))
    {
        e->GetComponent<TrailComponent>( )->SetVecToEnd( trailEnd );
    }
}

void HitscanWeaponSystem::GetSpawnLocation(const EntityHandle &other, physics::RaycastOutput &rayout, ursine::SVec3 &posToSet)
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

void HitscanWeaponSystem::SpawnCollisionParticle(SVec3 &collisionPoint, SVec3 &raycastVec, const EntityHandle &other)
{
    physics::RaycastInput rayin; // input for raycast check
    physics::RaycastOutput rayout; // output from raycast check

    raycastVec.Normalize( );

    // raycast info
    rayin.start = collisionPoint;
    rayin.end = rayin.start + (EXTRA_DIST * raycastVec);

    // get ray to edge of other object
    m_physicsSystem->Raycast( rayin, rayout, physics::RAYCAST_ALL_HITS, false, 1.0f, false );

    GetSpawnLocation( other->GetRoot( ), rayout, collisionPoint );
}
