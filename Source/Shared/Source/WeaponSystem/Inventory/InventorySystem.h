#pragma once
/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** InventorySystem.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include <FilterSystem.h>
#include <Component.h>

//////////////////////
////  Predefines  ////
//////////////////////

struct Inventory;
//////////////////////
//////////////////////


class InventorySystem
    : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;

public:
    InventorySystem(ursine::ecs::World *world);

protected:
    void onUpdate(EVENT_HANDLER(World)) override;
    void Enable(const ursine::ecs::EntityHandle &entity) override;
    void Disable(const ursine::ecs::EntityHandle &entity) override;

private:
    // check if inventory needs to have things updated
    void EvaluateInventory(Inventory* inventory);

    // Swap Weapons logic
    void SwapWeapons(Inventory* inventory);

    // change current weapon (visually and logically)
    void ChangeCurrentWeapon(Inventory* inventory);

    // load new weapon archetype
    void LoadWeapon(Inventory* inventory);

    void ActivateWeapon(Inventory* inventory);

    // Deactivate previous weapon of swap
    void DeactivateWeapon(Inventory* inventory, const int index);

    std::unordered_map<ursine::ecs::EntityHandle, Inventory*> m_inventories;
} Meta(Enable, AutoAddEntitySystem);
