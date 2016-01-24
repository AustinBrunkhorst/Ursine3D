
/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BaseWeaponSystem.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include "Precompiled.h"
#include "BaseWeaponSystem.h"
#include "BaseWeaponComponent.h"
#include "DamageOnCollideComponent.h"
#include "ProjectileComponent.h"
#include <RigidbodyComponent.h>
#include <TransformComponent.h>
#include <World.h>
#include <SystemConfig.h>
#include <Core/CoreSystem.h>
#include <SVec3.h>

ENTITY_SYSTEM_DEFINITION( BaseWeaponSystem );

using namespace ursine;
using namespace ursine::ecs;

namespace
{
    void ProjectileSetUp(ursine::ecs::Entity& proj, const BaseWeapon& weapon)
    {
        // add projectile component if not found
        if ( !proj.HasComponent<Projectile>( ) )
        {
            proj.AddComponent<Projectile>( );
        }

        // add damage on collide component if not found
        if ( !proj.HasComponent<DamageOnCollide>( ) )
        {
            proj.AddComponent<DamageOnCollide>( );
        }

        // add rigidbody component if not found
        if ( !proj.HasComponent<ursine::ecs::Rigidbody>( ) )
        {
            proj.AddComponent<ursine::ecs::Rigidbody>( );
        }

        Projectile& projComp = *proj.GetComponent<Projectile>( );
        // set proj speed
        projComp.SetSpeed(weapon.GetProjSpeed( ));
        // calc the lifet of proj base on weapons range
        projComp.CalculateLifeTime(weapon.GetMaxRange( ));

        // set proj damage stats
        DamageOnCollide& damageComp = *proj.GetComponent<DamageOnCollide>( );
        damageComp.SetDamageToApply(weapon.GetDamageToApply( ));
        damageComp.SetCritModifier(weapon.GetCritModifier( ));
        damageComp.SetDamageInterval(weapon.GetDamageInterval( ));
        damageComp.SetDeleteOnCollision(weapon.GetDeleteOnCollision( ));

        // give projectile velocity and position
        ursine::SVec3 forwardVec = weapon.GetOwner( )->GetTransform( )->GetForward( );
        proj.GetComponent<ursine::ecs::Rigidbody>( )->SetVelocity(forwardVec * projComp.GetSpeed( ));
        proj.GetTransform( )->SetWorldPosition(weapon.GetOwner( )->GetTransform( )->GetWorldPosition( ));
    }
}


BaseWeaponSystem::BaseWeaponSystem( ursine::ecs::World* world ) 
    : FilterSystem( world, ursine::ecs::Filter( ).All<BaseWeapon>( ) )
    , m_dt( 0 )
{
}

void BaseWeaponSystem::OnInitialize( void )
{
    FilterSystem::OnInitialize( );
}

void BaseWeaponSystem::Begin( )
{
    m_dt = Application::Instance->GetDeltaTime( );
}

void BaseWeaponSystem::Process( ursine::ecs::Entity * entity )
{
    URSINE_TODO( "Check if Process can get entities" );
    BaseWeapon& weapon = *entity->GetComponent<BaseWeapon>( );

    EvaluateWeapon( weapon );
}

void BaseWeaponSystem::EvaluateWeapon( BaseWeapon& weapon ) 
{
    URSINE_TODO( "Find way to store delta time" );

    // Can weapon be fired
    int fireStatus = weapon.CanFire( );

    switch ( fireStatus )
    {
    case RELOAD_IN_PROCESS:
        weapon.DecrementReloadTimer( m_dt );
        break;
    case MUST_RELOAD:
        ReloadWeapon( weapon );
        break;
    case FIRE_TIMER_SET:
        weapon.DecrementFireTimer( m_dt );
        break;
    case TRIGGER_NOT_PULLED:
        break;
    case CAN_FIRE:
        ShootWeapon( weapon );
        break;
    default:
        break;
    }

}

void BaseWeaponSystem::ShootWeapon( BaseWeapon& weapon )
{
    printf( "Shot Weapon\n " );
    // set weapons fire timer and get number of bullets fired
    int projectilesFired = weapon.Fire( );

    CreateProjectiles( weapon, projectilesFired);
}

void BaseWeaponSystem::ReloadWeapon( BaseWeapon& weapon ) const
{
    URSINE_TODO( "Have to apply reload animation" );

    printf( "Reload Started\n " );

    if ( weapon.Reload( ) )
    {

    }
}

void BaseWeaponSystem::CreateProjectiles( BaseWeapon& weapon, int projectilesFired)
{

    // Create the projectile that is desired to shoot
    ursine::ecs::Entity* proj =
        m_world->CreateEntityFromArchetype(WORLD_ARCHETYPE_PATH + weapon.GetArchetypeToShoot( ), "Bullet");

    // set up projectile stats based on gun
    ProjectileSetUp( *proj, weapon );

    // create the number of projectiles fired
    for ( int i = 1; i < projectilesFired; ++i )
    {
        proj->Clone( );
    }

}



