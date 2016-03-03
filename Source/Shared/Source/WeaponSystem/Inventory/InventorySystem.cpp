
/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** InventorySystem.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include "Precompiled.h"
#include "InventorySystem.h"
#include "GameEvents.h"
#include "InventoryComponent.h"
#include "HitscanWeaponComponent.h"
#include "AbstractWeapon.h"
#include <AnimatorComponent.h>
#include "FirePosComponent.h"


ENTITY_SYSTEM_DEFINITION( InventorySystem ) ;

using namespace ursine;
using namespace ursine::ecs;

namespace
{

} // unnamed namespace


////////////////////////////
////  Inventory System  ////
////////////////////////////

InventorySystem::InventorySystem(ursine::ecs::World* world)
    : FilterSystem( world, Filter( ).One< Inventory >( ) )
{
}


void InventorySystem::Enable(ursine::ecs::Entity* entity)
{
    auto uniqueID = entity->GetUniqueID( );

    // grab all comps needed
    if ( entity->HasComponent< Inventory >( ) )
        m_inventories[ uniqueID ] = entity->GetComponent< Inventory >( );
}

void InventorySystem::Disable(ursine::ecs::Entity* entity)
{
    auto uniqueID = entity->GetUniqueID( );

    m_inventories.erase( uniqueID );
}

void InventorySystem::onUpdate(EVENT_HANDLER(World))
{
    for ( auto inventory : m_inventories )
    {
        EvaluateInventory(&*inventory.second);
    }
}


void InventorySystem::EvaluateInventory(Inventory* inventory)
{
    if ( !inventory->m_init )
        inventory->Init( );

    SwapWeapons( inventory );

    ChangeCurrentWeapon( inventory );
}

void InventorySystem::SwapWeapons(Inventory* inventory)
{
    // check if swap was trigged
    if ( inventory->m_swap )
    {
        int i = ( inventory->m_currWeapon + 1 ) % Inventory::INVENTORY_COUNT;

        // walk through weapons and grab next valid weapon
        for ( ; i != inventory->m_currWeapon; ++i )
        {
            i = i % Inventory::INVENTORY_COUNT;

            // search for next active gun
            if ( inventory->m_inventory[ i ].m_weaponLoaded )
            {
                DeactivateWeapon( inventory, i );
                inventory->m_newWeapon = true;
                break;
            }
        }

        // trying to change weapons w/ no other active weapons
        if ( inventory->m_prevWeapon == inventory->m_currWeapon )
        {
            URSINE_TODO("Give feed back that player cannot switch weapons");
        }
    }
}


void InventorySystem::ChangeCurrentWeapon(Inventory* inventory)
{
    // did we recieve a new weapon or swap weapons
    if ( inventory->m_newWeapon )
    {
        if ( !inventory->m_swap && inventory->m_inventory[ inventory->m_prevWeapon ].m_weaponLoaded )
        {
            // detatch current weapon
            game::WeaponDeactivationEventArgs args( inventory->GetOwner( ) );
            inventory->m_inventory[ inventory->m_prevWeapon ].m_weaponLoaded->Dispatch(game::DETACH_WEAPON, &args);
        }

        // update previous
        inventory->m_prevWeapon = inventory->m_currWeapon;

        LoadWeapon( inventory );
    }
} 
 
void InventorySystem::LoadWeapon(Inventory* inventory)
{
    WeaponSlotInfo& weaponSlot = inventory->m_inventory[ inventory->m_currWeapon ];

    // create weapon
    weaponSlot.m_weaponLoaded = m_world->CreateEntityFromArchetype( WORLD_ARCHETYPE_PATH + weaponSlot.m_weaponToLoad, "Weapoon" );

    // activate weapon and grab spawn offset
    ActivateWeapon(inventory);

    // Parent weapon to arm
    inventory->m_cameraHandle->AddChildAlreadyInLocal( weaponSlot.m_weaponLoaded->GetTransform( ) );

    inventory->m_newWeapon = false;
}


void InventorySystem::ActivateWeapon(Inventory* inventory)
{
    //create event args
    game::WeaponActivationEventArgs args( inventory->GetOwner( ), inventory->m_cameraHandle );

    // set position
    ursine::ecs::Transform* trans = inventory->m_inventory[ inventory->m_currWeapon ].m_weaponLoaded->GetTransform( );
    trans->SetWorldPosition(trans->GetLocalPosition( ));
    
    // was weapon swapped in (if so give it its' previous stats)
    if ( inventory->m_swap )
    {
        args.m_ammo = inventory->m_inventory[ inventory->m_currWeapon ].m_ammoCount;
        args.m_clip = inventory->m_inventory[ inventory->m_currWeapon ].m_clipCount;

        inventory->m_swap = false;
    }
    
    // activate current weapon
    inventory->m_inventory[ inventory->m_currWeapon ].m_weaponLoaded->Dispatch(game::ACTIVATE_WEAPON, &args);
}

void InventorySystem::DeactivateWeapon(Inventory* inventory, const int index)
{
    // change curr weapon to newly found weapon in inventory
    inventory->m_currWeapon = inventory->m_inventory[ index ].m_weaponSlotType;

    //create event args
    game::WeaponDeactivationEventArgs args( inventory->GetOwner( ) );

    // deactivate weapon
    inventory->m_inventory[ inventory->m_prevWeapon ].m_weaponLoaded->Dispatch(game::DEACTIVATE_WEAPON, &args);

    // update ammo and clip info
    inventory->m_inventory[ inventory->m_prevWeapon ].m_ammoCount = args.m_ammo;
    inventory->m_inventory[ inventory->m_prevWeapon ].m_clipCount = args.m_clip;

    // delete the weapon
    inventory->m_inventory[ inventory->m_prevWeapon ].m_weaponLoaded->Delete( );
}



