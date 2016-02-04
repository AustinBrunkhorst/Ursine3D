
/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
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
    if ( inventory->m_currWeapon != inventory->m_prevWeapon )
    {
        if ( !inventory->m_swap )
        {
            // activate current weapon
            inventory->m_inventory[ inventory->m_prevWeapon ].m_weaponLoaded->Dispatch(game::DETACH_WEAPON, ursine::ecs::EntityEventArgs::Empty);
        }

        // update previous
        inventory->m_prevWeapon = inventory->m_currWeapon;

        LoadWeapon( inventory );
    }
} 
 
void InventorySystem::LoadWeapon(Inventory* inventory)
{
    Inventory::WeaponSlotInfo& weaponSlot = inventory->m_inventory[ inventory->m_currWeapon ];

    // create weapon
    weaponSlot.m_weaponLoaded = m_world->CreateEntityFromArchetype( weaponSlot.m_weaponToLoad );

    ursine::ecs::Transform* weaponsTrans = weaponSlot.m_weaponLoaded->GetTransform( );
    // move weapon to arm
    weaponsTrans->SetWorldPosition( inventory->m_armHandle->GetWorldPosition( ) );

    // activate weapon and grab spawn offset
    ActivateWeapon(inventory, weaponsTrans);

    // set rotation of arm
    weaponsTrans->SetWorldRotation( inventory->m_armHandle->GetWorldRotation( ) );

    // Parent weapon to arm
    inventory->m_armHandle->AddChild( weaponSlot.m_weaponLoaded->GetTransform( ) );
}


void InventorySystem::ActivateWeapon(Inventory* inventory, ursine::ecs::Transform* trans)
{
    //create event args
    game::WeaponActivationEventArgs args(&inventory->m_cameraHandle);

    // was weapon swapped in (if so give it its' previous stats)
    if ( inventory->m_swap )
    {
        args.m_ammo = inventory->m_inventory[ inventory->m_currWeapon ].m_ammoCount;
        args.m_clip = inventory->m_inventory[ inventory->m_currWeapon ].m_clipCount;

        inventory->m_swap = false;
    }

    // activate current weapon
    inventory->m_inventory[ inventory->m_currWeapon ].m_weaponLoaded->Dispatch(game::ACTIVATE_WEAPON, &args);

    // set spawn offset
    args.spawnOffset = trans->GetWorldRotation( ) * args.spawnOffset;
    trans->SetWorldPosition( trans->GetWorldPosition( ) + args.spawnOffset );
}


void InventorySystem::DeactivateWeapon(Inventory* inventory, const int index)
{
    // change curr weapon to newly found weapon in inventory
    inventory->m_currWeapon = inventory->m_inventory[ index ].m_weaponSlotType;

    //create event args
    game::WeaponDeactivationEventArgs args;

    // deactivate weapon
    inventory->m_inventory[ inventory->m_prevWeapon ].m_weaponLoaded->Dispatch(game::DEACTIVATE_WEAPON, &args);

    // update ammo and clip info
    inventory->m_inventory[ inventory->m_prevWeapon ].m_ammoCount = args.m_ammo;
    inventory->m_inventory[ inventory->m_prevWeapon ].m_clipCount = args.m_clip;

    // delete the weapon
    inventory->m_inventory[ inventory->m_prevWeapon ].m_weaponLoaded->Delete( );
}



