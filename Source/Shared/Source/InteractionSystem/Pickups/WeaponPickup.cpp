/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WeaponPickup.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "WeaponPickup.h"
#include "ComponentIncludes.h"
#include "CommandQueueComponent.h"
#include "InventoryComponent.h"
#include "InteractableComponent.h"

NATIVE_COMPONENT_DEFINITION( WeaponPickup );

using namespace ursine;

WeaponPickup::WeaponPickup(void) 
    : BaseComponent( )
    , m_pickupTime( 0.5f )
    , m_ammo( -1 )
    , m_clipCount( -1 )
    , m_weaponType( PRIMARY_WEAPON )
    , m_weaponToPickup( "BaseWeapon.uatype" )
    , m_texture( "" ) { }

WeaponPickup::~WeaponPickup(void)
{
}

void WeaponPickup::OnInitialize(void)
{
    GetOwner( )->GetComponent<Interactable>( )->SetUpInteractable(this);
}

///////////////////////////////
////  Gettors and Settors  ////
///////////////////////////////

// pickup time
float WeaponPickup::GetPickupTime(void) const
{
    return m_pickupTime;
}

void WeaponPickup::SetPickupTime(const float time)
{
    m_pickupTime = time;
}


// Weapon type of pick up
WeaponType WeaponPickup::GetWeaponType( ) const
{
    return m_weaponType;
}

void WeaponPickup::SetWeaponType(WeaponType weapon)
{
    m_weaponType = weapon;
}


// WeaponToPickup
const std::string& WeaponPickup::GetWeaponToPickup( ) const
{
    return m_weaponToPickup;
}

void WeaponPickup::SetWeaponToPickup(const std::string& weapon)
{
    m_weaponToPickup = weapon;

    // add on extension for archetypes
    if ( m_weaponToPickup.find(".uatype") == std::string::npos )
        m_weaponToPickup += ".uatupe";
}


// Texture
const std::string& WeaponPickup::GetTexture( ) const
{
    return m_texture;
}

void WeaponPickup::SetTexture(const std::string& texture)
{
    m_texture = texture;
}


///////////////////////////////
////  Weapon Pickup Logic  ////
///////////////////////////////

void WeaponPickup::SetAmmoInfo(const int ammo, const int clip)
{
    m_ammo = ammo;
    m_clipCount = clip;
}

void WeaponPickup::StartInteraction(const CommandQueue *queue, ursine::ecs::EntityHandle &entity)
{
    // get invetory
    m_inventories[ entity ] = queue->GetOwner( )->GetComponent<Inventory>( );

    //if ( CheckForAmmo( id ) )
    //    return;

    // start time
    m_times[ entity ] = ursine::Application::Instance->GetDeltaTime( );
}

void WeaponPickup::Interact(const CommandQueue *queue, ecs::EntityHandle &entity)
{
    Inventory* inventory = &*m_inventories[ entity ];

    if ( inventory == nullptr )
        return;

    // check reload time
    if ( queue->QueryCommand(game::INTERACT_COMMAND) )
    {
        // grab time
        float* time = &m_times[ entity ];

        // update
        *time += ursine::Application::Instance->GetDeltaTime( );

        // swap weapons if the required time for pickup has been met
        if ( *time > m_pickupTime && !GetOwner()->IsDeleting() )
        {
            // change current weapon 
            // @Chad, should this function really take an archetype reference from us? or should that be held within the inventory component?
            inventory->SetNewWeapon( m_weaponType, m_weaponToPickup, m_ammo, m_clipCount );

            InteractionComplete( );
        }
    }

    // interact not being held, so reset time
    else
    {
        m_times[ entity ] = 0.0f;
    }
}

void WeaponPickup::StopInteraction(const CommandQueue *queue, ecs::EntityHandle &entity)
{
    m_times.erase( entity );
    m_inventories.erase( entity );
}

void WeaponPickup::InteractionComplete(void)
{
    // kill thy self
    GetOwner( )->Delete( );
}

void WeaponPickup::CheckForAmmo(const ecs::EntityHandle &entity)
{
    Inventory* inventory = &*m_inventories[ entity ];

    if ( inventory == nullptr )
        return;

    WeaponSlotInfo& weaponInfo = inventory->m_inventory[ inventory->m_currWeapon ];

    if ( weaponInfo.m_weaponToLoad == m_weaponToPickup )
    {
       // if ( weaponInfo.m_weaponLoaded )
    }
}
