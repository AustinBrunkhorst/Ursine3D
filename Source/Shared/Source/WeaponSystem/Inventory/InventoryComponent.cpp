/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** InventoryComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "InventoryComponent.h"
#include "ComponentIncludes.h"
#include "CameraComponent.h"
#include "TransformComponent.h"

NATIVE_COMPONENT_DEFINITION( Inventory ) ;

using namespace ursine;
using namespace resources;

  //////////////////////////////////
  ////  WeaponSlotInfo Members  ////
  //////////////////////////////////

WeaponSlotInfo::WeaponSlotInfo(WeaponType index) 
    : m_weaponSlotType( index )
    , m_weaponLoaded( )
    , m_ammoCount( -1 )
    , m_clipCount( -1 ) { }

/////////////////////////////
////  Inventory Members  ////
/////////////////////////////

Inventory::Inventory(void) 
    : BaseComponent( )
    , m_currWeapon( LAST_STAND )
    , m_prevWeapon( LAST_STAND )
    , m_inventory { 
          WeaponSlotInfo( LAST_STAND ),
          WeaponSlotInfo( SECONDARY_WEAPON ),
          WeaponSlotInfo( PRIMARY_WEAPON ),
          WeaponSlotInfo( MELEE_WEAPON ),
          WeaponSlotInfo( GOD_WEAPON )
      }
    , m_swap( false )
    , m_newWeapon( true )
    , m_init( false )
{
}

Inventory::~Inventory(void)
{
    GetOwner( )->Listener(this)
        .Off(game::SWAP_COMMAND, &Inventory::TriggerSwapWeapons);
}

void Inventory::OnInitialize(void)
{
    // default positions of camera and arm to current position
    m_cameraHandle = GetOwner( )->GetTransform( );
    m_armHandle = GetOwner( )->GetTransform( );

    GetOwner( )->Listener(this)
        .On(game::SWAP_COMMAND, &Inventory::TriggerSwapWeapons);
}

void Inventory::Init(void)
{
    // try to get camera position
    auto *cam = GetOwner( )->GetComponentInChildren<ecs::Camera>( );

    if ( cam )
    {
        m_cameraHandle = cam->GetOwner( )->GetTransform( );

        // try to get right arm position
        auto arm = cam->GetOwner( )->GetChildByName( "RightArm" );

        if ( arm )
        {
            m_armHandle = arm->GetTransform( );
        }
    }

}

WeaponType Inventory::GetStartWeapon(void) const
{
    return m_currWeapon;
}

void Inventory::SetStartWeapon(const WeaponType slot)
{
    m_currWeapon = slot;
}

const ResourceReference &Inventory::GetLastStandWeapon(void) const
{
    return m_inventory[ LAST_STAND ].m_weaponToLoad;
}

void Inventory::SetLastStandWeapon(const ResourceReference &archetype)
{
    m_inventory[ LAST_STAND ].m_weaponToLoad = archetype;

    NOTIFY_COMPONENT_CHANGED( "lastStand", m_inventory[ LAST_STAND ].m_weaponToLoad );
}

const ResourceReference &Inventory::GetSecondaryWeapon(void) const
{
    return m_inventory[ SECONDARY_WEAPON ].m_weaponToLoad;
}

void Inventory::SetSecondaryWeapon(const ResourceReference &archetype)
{
    m_inventory[ SECONDARY_WEAPON ].m_weaponToLoad = archetype;

    NOTIFY_COMPONENT_CHANGED( "secondaryWeapon", m_inventory[ SECONDARY_WEAPON ].m_weaponToLoad );
}

const ResourceReference &Inventory::GetPrimaryWeapon(void) const
{
    return m_inventory[ PRIMARY_WEAPON ].m_weaponToLoad;
}

void Inventory::SetPrimaryWeapon(const ResourceReference &archetype)
{
    m_inventory[ PRIMARY_WEAPON ].m_weaponToLoad = archetype;

    NOTIFY_COMPONENT_CHANGED( "primaryWeapon", m_inventory[ PRIMARY_WEAPON ].m_weaponToLoad );
}

const ResourceReference &Inventory::GetMeleeWeapon(void) const
{
    return m_inventory[ MELEE_WEAPON ].m_weaponToLoad;
}

void Inventory::SetMeleeWeapon(const ResourceReference &archetype)
{
    m_inventory[ MELEE_WEAPON ].m_weaponToLoad = archetype;

    NOTIFY_COMPONENT_CHANGED( "meleeWeapon", m_inventory[ MELEE_WEAPON ].m_weaponToLoad );
}

void Inventory::SetNewWeapon(const ResourceReference &weaponData, const WeaponType type, int ammo, int clip)
{
    // @Chad, set the weapon based on the weaponData

    m_inventory[ type ].m_ammoCount = ammo;
    m_inventory[ type ].m_clipCount = clip;
    m_currWeapon = type;

    m_newWeapon = true;
}

void Inventory::TriggerSwapWeapons(void* _sender, const ursine::EventArgs* _args)
{
    m_swap = true;
}

//
///////////////////////////////
//////  Inventory Members  ////
///////////////////////////////
//
//Inventory2::Inventory2(void) :
//    BaseComponent( ),
//    m_currWeapon(PRIMARY_WEAPON),
//    m_prevWeapon(PRIMARY_WEAPON),
//    m_inventory{
//    WeaponSlotInfo(SECONDARY_WEAPON),
//    WeaponSlotInfo(PRIMARY_WEAPON,"HitscanWeapon.uatype"),
//    WeaponSlotInfo(GOD_WEAPON)
//},
//m_swap(false),
//m_newWeapon(true),
//m_init(false)
//{
//}
//
//void Inventory2::OnInitialize(void)
//{
//
//    // default positions of camera and arm to current position
//    m_cameraHandle = GetOwner( )->GetTransform( );
//    m_armHandle = GetOwner( )->GetTransform( );
//}
//
//void Inventory2::Init(void)
//{
//    // try to get camera position
//    ursine::ecs::Camera* cam = GetOwner( )->GetComponentInChildren<ursine::ecs::Camera>( );
//
//    if ( cam )
//    {
//        m_cameraHandle = cam->GetOwner( )->GetTransform( );
//
//        // try to get right arm position
//        ursine::ecs::Entity* arm = cam->GetOwner( )->GetChildByName("RightArm");
//
//        if ( arm )
//        {
//            m_armHandle = arm->GetTransform( );
//        }
//    }
//
//}
//
//WeaponType Inventory2::GetStartWeapon(void) const
//{
//    return m_currWeapon;
//}
//
//void Inventory2::SetStartWeapon(const WeaponType slot)
//{
//    m_currWeapon = slot;
//}
//
//
//const std::string& Inventory2::GetSecondaryWeapon( ) const
//{
//    return m_inventory[ SECONDARY_WEAPON ].m_weaponToLoad;
//}
//
//void Inventory2::SetSecondaryWeapon(const std::string& archetype)
//{
//    if ( SetWeaponSlot(m_inventory[ SECONDARY_WEAPON ], archetype) )
//        m_newWeapon = true;
//}
//
//const std::string& Inventory2::GetPrimaryWeapon( ) const
//{
//    return m_inventory[ PRIMARY_WEAPON ].m_weaponToLoad;
//}
//
//void Inventory2::SetPrimaryWeapon(const std::string& archetype)
//{
//    if ( SetWeaponSlot(m_inventory[ PRIMARY_WEAPON ], archetype) )
//        m_newWeapon = true;
//}
//
//
//void Inventory2::SetNewWeapon(const WeaponType type, const std::string& weaponToLoad, int ammo, int clip)
//{
//    m_inventory[ type ].m_weaponToLoad = weaponToLoad;
//    m_inventory[ type ].m_ammoCount = ammo;
//    m_inventory[ type ].m_clipCount = clip;
//    m_currWeapon = type;
//
//    m_newWeapon = true;
//}
//
//void Inventory2::TriggerSwapWeapons(void* _sender, const ursine::EventArgs* _args)
//{
//    m_swap = true;
//}
//


