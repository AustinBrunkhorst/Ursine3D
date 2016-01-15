
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
#include <RigidbodyComponent.h>
#include <TransformComponent.h>
#include <World.h>
#include <SystemConfig.h>
#include <Core/CoreSystem.h>
#include <Core/Input/Keyboard/KeyboardManager.h>
#include <Core/Input/Mouse/MouseManager.h>
#include <SVec3.h>

ENTITY_SYSTEM_DEFINITION( BaseWeaponSystem );

using namespace ursine;
using namespace ursine::ecs;


BaseWeaponSystem::BaseWeaponSystem( ursine::ecs::World* world ) :
    FilterSystem( world, ursine::ecs::Filter( ).All<BaseWeapon>( ) )
{
}

void BaseWeaponSystem::OnInitialize( void )
{
    //m_world->CreateEntityFromArchetype(std::string("bh"), "Bullet");

    FilterSystem::OnInitialize( );
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
        weapon.DecrementReloadTimer( Application::Instance->GetDeltaTime( ) );
        printf( "Reload in progress\n " );
        break;
    case MUST_RELOAD:
        weapon.Reload( );
        printf( "Reload Started\n " );
        break;
    case FIRE_TIMER_SET:
        weapon.DecrementFireTimer( Application::Instance->GetDeltaTime( ) );
        printf( "Fire Timer Set\n " );
        break;
    case TRIGGER_NOT_PULLED:
        break;
    case CAN_FIRE:
        ShootWeapon( weapon );
        printf( "Shot Weapon\n " );
        break;
    default:
        break;
    }

}

void BaseWeaponSystem::ShootWeapon( BaseWeapon& weapon )
{
    // set weapons fire timer and get number of bullets fired
    int bulletsFired = weapon.Fire( );

    CreateBullets( weapon, bulletsFired );
}

void BaseWeaponSystem::ReloadWeapon( BaseWeapon& weapon )
{
    URSINE_TODO( "Have to apply reload animation" );

    if ( weapon.Reload( ) )
    {

    }
}

void BaseWeaponSystem::CreateBullets( BaseWeapon& weapon, int bulletsFired )
{
    // create the number of rounds fired
    for ( int i = 0; i < bulletsFired; ++i )
    {
        ursine::ecs::Entity* bullet =
            m_world->CreateEntityFromArchetype( WORLD_ARCHETYPE_PATH + weapon.GetArchetypeToShoot( ), "Bullet" );

        bullet->GetComponent<ursine::ecs::Rigidbody>( )->AddForce( ursine::SVec3( 0, 100.0f, 10.0f ) );
        bullet->GetTransform( )->SetWorldPosition( weapon.GetOwner( )->GetTransform( )->GetWorldPosition( ) );
    }

}



