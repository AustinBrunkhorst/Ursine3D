#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WeaponPickup.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Component.h>
#include <ArchetypeData.h>

#include "BaseInteraction.h"
#include "AbstractWeapon.h"

struct Inventory;

struct WeaponPickup 
    : ursine::ecs::Component
    , Interaction
{
    NATIVE_COMPONENT
public:

    WeaponPickup(void);
    ~WeaponPickup(void);

    EditorField(
        float PickupTime,
        GetPickupTime,
        SetPickupTime
    );

    float GetPickupTime(void) const;
    void SetPickupTime(const float time);

    EditorField(
        WeaponType weaponType,
        GetWeaponType,
        SetWeaponType
    );

    WeaponType GetWeaponType(void) const;
    void SetWeaponType(WeaponType weaponType);

    EditorResourceField(
        ursine::resources::ArchetypeData,
        Weapon,
        GetWeaponToPickup,
        SetWeaponToPickup
    );

    const ursine::resources::ResourceReference& GetWeaponToPickup(void) const;
    void SetWeaponToPickup(const ursine::resources::ResourceReference& weapon);

    EditorField(
        std::string Texture,
        GetTexture,
        SetTexture
    );

    const std::string& GetTexture(void) const;
    void SetTexture(const std::string& texture);

    void SetAmmoInfo(const int ammo, const int clip);

private:
    // interaction logic
    void StartInteraction(const CommandQueue* queue, ursine::ecs::EntityHandle &entity) override;
    void Interact(const CommandQueue* queue, ursine::ecs::EntityHandle &entity) override;
    void StopInteraction(const CommandQueue* queue, ursine::ecs::EntityHandle &entity) override;
    void InteractionComplete(void);
    void CheckForAmmo(const ursine::ecs::EntityHandle &entity);


    // time it takes to pick up
    float m_pickupTime;

    // ammo to give weapon (-1 means go with archetype value)
    int m_ammo;

    // clip count to give weapon (-1 means go with archetype value)
    int m_clipCount;

    // type of weapon pickup
    WeaponType m_weaponType;

    // how much time has button been held
    std::unordered_map<ursine::ecs::EntityHandle, float> m_times;

    // inventory handles
    std::unordered_map<ursine::ecs::EntityHandle, Inventory*> m_inventories;

    // Weapon the will be picked up on completed interaction
    ursine::resources::ResourceReference m_weaponToPickup;

    // Texture to display to object
    std::string m_texture;

    void OnInitialize(void) override;
} Meta(
    Enable, 
    DisplayName( "WeaponPickup" ), 
    RequiresComponents( typeof( Interactable ) )
);

