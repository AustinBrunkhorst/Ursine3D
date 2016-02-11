/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

namespace
{
    // sets the archetype that is desired to load
    bool SetWeaponSlot(Inventory::WeaponSlotInfo& weaponSlot, const std::string& weaponToLoad)
    {
        bool isValid = false;

        // update weapon to load
        if ( weaponToLoad != "" )
        {
            weaponSlot.m_weaponToLoad = weaponToLoad;

            // add on extension for archetypes
            if ( weaponToLoad.find(".uatype") == std::string::npos )
            {
                weaponSlot.m_weaponToLoad += ".uatupe";
                isValid = true;
            }
        }

        return isValid;
    }

} // unnamed namespace


  //////////////////////////////////
  ////  WeaponSlotInfo Members  ////
  //////////////////////////////////

Inventory::WeaponSlotInfo::WeaponSlotInfo(WeaponType index, const std::string& weapon) :
    m_weaponSlotType( index ),
    m_weaponLoaded( nullptr ),
    m_weaponToLoad( weapon ),
    m_ammoCount( -1 ),
    m_clipCount( -1 )
{
    SetWeaponSlot( *this, weapon );
}




/////////////////////////////
////  Inventory Members  ////
/////////////////////////////

Inventory::Inventory(void) :
    BaseComponent( ), 
    m_currWeapon( LAST_STAND ),
    m_prevWeapon(LAST_STAND),
    m_inventory { 
                    WeaponSlotInfo( LAST_STAND, "HitscanWeapon.uatype"),
                    WeaponSlotInfo( SECONDARY_WEAPON ),
                    WeaponSlotInfo( PRIMARY_WEAPON ),
                    WeaponSlotInfo( MELEE_WEAPON ),
                    WeaponSlotInfo( GOD_WEAPON )
                },
    m_swap( false ),
    m_newWeapon( true ),
    m_init( false )
{
}

void Inventory::OnInitialize(void)
{
    // default positions of camera and arm to current position
    m_cameraHandle = GetOwner( )->GetTransform( );
    m_armHandle = GetOwner( )->GetTransform( );
}

void Inventory::Init(void)
{
    // try to get camera position
    ursine::ecs::Camera* cam = GetOwner( )->GetComponentInChildren<ursine::ecs::Camera>( );

    if ( cam )
    {
        m_cameraHandle = cam->GetOwner( )->GetTransform( );

        // try to get right arm position
        ursine::ecs::Entity* arm = cam->GetOwner( )->GetChildByName("RightArm");

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

const std::string& Inventory::GetLastStandWeapon( ) const
{
    return m_inventory[ LAST_STAND ].m_weaponToLoad;
}

void Inventory::SetLastStandWeapon(const std::string& archetype)
{
    if ( SetWeaponSlot( m_inventory[ LAST_STAND ], archetype ) )
        m_newWeapon = true;
}

const std::string& Inventory::GetSecondaryWeapon( ) const
{
    return m_inventory[ SECONDARY_WEAPON ].m_weaponToLoad;
}

void Inventory::SetSecondaryWeapon(const std::string& archetype)
{
    if ( SetWeaponSlot(m_inventory[ SECONDARY_WEAPON ], archetype) )
        m_newWeapon = true;
}

const std::string& Inventory::GetPrimaryWeapon( ) const
{
    return m_inventory[ PRIMARY_WEAPON ].m_weaponToLoad;
}

void Inventory::SetPrimaryWeapon(const std::string& archetype)
{
    if ( SetWeaponSlot(m_inventory[ PRIMARY_WEAPON ], archetype) )
        m_newWeapon = true;
}

const std::string& Inventory::GetMeleeWeapon( ) const
{
    return m_inventory[ MELEE_WEAPON ].m_weaponToLoad;
}

void Inventory::SetMeleeWeapon(const std::string& archetype)
{
    if ( SetWeaponSlot( m_inventory[ MELEE_WEAPON ], archetype ) )
        m_newWeapon = true;
}

void Inventory::SetNewWeapon(const WeaponType type, const std::string& weaponToLoad, int ammo, int clip)
{
    m_inventory[ type ].m_weaponToLoad = weaponToLoad;
    m_inventory[ type ].m_ammoCount = ammo;
    m_inventory[ type ].m_clipCount = clip;
    m_currWeapon = type;

    m_newWeapon = true;
}

void Inventory::TriggerSwapWeapons(void* _sender, const ursine::EventArgs* _args)
{
    m_swap = true;
}
