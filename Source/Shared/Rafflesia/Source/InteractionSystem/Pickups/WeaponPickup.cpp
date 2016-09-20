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
#include "PlayerIdComponent.h"

NATIVE_COMPONENT_DEFINITION( WeaponPickup );

using namespace ursine;


namespace gameUIEvents
{
    const auto UI_HealthComponentStats = "UI_HealthComponentStats";
}


WeaponPickup::WeaponPickup(void) 
    : BaseComponent( )
    , m_pickupTime( 0.5f )
    , m_ammo( -1 )
    , m_clipCount( -1 )
    , m_weaponType( PRIMARY_WEAPON )
{ }

WeaponPickup::~WeaponPickup(void)
{
}

void WeaponPickup::OnInitialize(void)
{
    GetOwner( )->GetComponent<Interactable>( )->SetUpInteractable( this, Interactable::END );
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
const ursine::resources::ResourceReference& WeaponPickup::GetWeaponToPickup(void) const
{
    return m_weaponToPickup;
}

void WeaponPickup::SetWeaponToPickup(const ursine::resources::ResourceReference& weapon)
{
    m_weaponToPickup = weapon;

    NOTIFY_COMPONENT_CHANGED("Weapon", m_weaponToPickup);
}

// Texture
const ursine::resources::ResourceReference& WeaponPickup::GetTexture(void) const
{
    return m_texture;
}

void WeaponPickup::SetTexture(const ursine::resources::ResourceReference& texture)
{
    m_texture = texture;

    NOTIFY_COMPONENT_CHANGED("Texture", m_texture);
}

///////////////////////////////
////  Weapon Pickup Logic  ////
///////////////////////////////

void WeaponPickup::SetAmmoInfo(const int ammo, const int clip)
{
    m_ammo = ammo;
    m_clipCount = clip;
}

void WeaponPickup::StartInteraction(const ursine::ecs::EntityHandle &entity)
{
    // return if not player or object deleting
    if ( GetOwner( )->IsDeleting( ) || !entity->HasComponent<PlayerID>( ) )
        return;

    if ( entity->HasComponent<Inventory>( ) )
    {
        auto inventory = entity->GetComponent<Inventory>( );
        auto weaponSlot = inventory->m_inventory[ inventory->m_currWeapon ];
        
        // is weapon a weapon in inventory the same as pickup weapon
        for ( int i = 0; i < WeaponType::GOD_WEAPON + 1; ++i )
        {
            // weapons not the same
            if ( !( m_weaponToPickup == weaponSlot.m_weaponToLoad ) )
                continue;

            // add ammo to weapon in inventory
            weaponSlot.m_ammoCount += m_ammo;

            // if weapon is active, add ammo directly
            if ( i == inventory->m_currWeapon )
            {
                // have object pick up ammo since weapons the same
                game::AmmoPickupEventArgs args(m_ammo);
                weaponSlot.m_weaponLoaded->Dispatch(game::PICKUP_AMMO, &args);
            }

            InteractionComplete( );

            return;
        }
    }

    // get invetory
    m_inventories[ entity ] = entity->GetComponent<Inventory>( );

    // get command queue
    m_queues[ entity ] = entity->GetComponent<CommandQueue>( );

    // start time
    m_times[ entity ] = ursine::Application::Instance->GetDeltaTime( );

    // display pickup message
    URSINE_TODO("Figure out json issue")
    //ursine::Json message = ursine::Json::object{
    //    { "playerID", entity->GetComponent< PlayerID >( )->GetID( ) },
    //    { "PickUpTexture", m_texture }
    //};
    //
    //GetOwner( )->GetWorld( )->MessageUI(gameUIEvents::UI_HealthComponentStats, message);
}

void WeaponPickup::Interact(const ecs::EntityHandle &entity)
{
    Inventory* inventory = m_inventories[ entity ];
    CommandQueue* queue = m_queues[ entity ];

    if ( inventory == nullptr || queue == nullptr || GetOwner( )->IsDeleting( ) )
        return;

    // check reload time
    if ( queue->QueryCommand(game::INTERACT_COMMAND) )
    {
        // grab time
        float* time = &m_times[ entity ];

        // update
        *time += ursine::Application::Instance->GetDeltaTime( );

        // swap weapons if the required time for pickup has been met
        if ( *time > m_pickupTime )
        {
            // change current weapon 
            inventory->SetNewWeapon( m_weaponToPickup, m_weaponType, m_ammo, m_clipCount );

            InteractionComplete( );
        }
    }

    // interact not being held, so reset time
    else
    {
        m_times[ entity ] = 0.0f;
    }
}

void WeaponPickup::StopInteraction(const ecs::EntityHandle &entity)
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

    // @Chad, what to do with this?
    //if ( weaponInfo.m_weaponToLoad == m_weaponToPickup )
    {
       // if ( weaponInfo.m_weaponLoaded )
    }
}
