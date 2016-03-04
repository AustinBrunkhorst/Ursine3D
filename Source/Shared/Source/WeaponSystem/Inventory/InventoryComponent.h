#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** InventoryComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/


#include <Component.h>
#include <SVec3.h>
#include "AbstractWeapon.h"

#include <ArchetypeData.h>

namespace ursine
{
    namespace ecs
    {
        class Transform;
    } // ecs namespace

} // ursine namespace

struct WeaponSlotInfo
{
    WeaponSlotInfo(WeaponType slotType, const std::string& weapon = "");

    // index into inventory
    WeaponType m_weaponSlotType;

    // loaded weapon archetype
    //   give ease of access
    ursine::ecs::EntityHandle m_weaponLoaded;

    // weapon archetype that will be loaded when ever this weapon slot is active
    std::string m_weaponToLoad;

    // how much ammo was in gun before being deactivated
    int m_ammoCount;

    // how much did clip contain before being deactivated
    int m_clipCount;
};

struct Inventory : ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    static const int INVENTORY_COUNT = 4;

    Inventory(void);
    ~Inventory(void);

    void OnInitialize(void) override;
    void Init(void);

    EditorField(
        WeaponType StartWeapon,
        GetStartWeapon,
        SetStartWeapon
    );

    EditorField(
        std::string LastStand,
        GetLastStandWeapon,
        SetLastStandWeapon
    );
   
    EditorField(
        std::string SecondaryWeapon,
        GetSecondaryWeapon,
        SetSecondaryWeapon
    );

    EditorField(
        std::string PrimaryWeapon,
        GetPrimaryWeapon,
        SetPrimaryWeapon
    );

    EditorField(
        std::string MeleeWeapon,
        GetMeleeWeapon,
        SetMeleeWeapon
    );

    WeaponType GetStartWeapon(void) const;
    void SetStartWeapon(const WeaponType slot);

    const std::string& GetLastStandWeapon(void) const;
    void SetLastStandWeapon(const std::string& archetype);

    const std::string& GetSecondaryWeapon(void) const;
    void SetSecondaryWeapon(const std::string& archetype);

    const std::string& GetPrimaryWeapon(void) const;
    void SetPrimaryWeapon(const std::string& archetype);

    const std::string& GetMeleeWeapon(void) const;
    void SetMeleeWeapon(const std::string& archetype);

    void SetNewWeapon(const WeaponType type, const std::string& weaponToLoad, int ammo, int clip);


    ///////////////////
    ////  Members  ////
    ///////////////////

    // current weapon out
    WeaponType m_currWeapon;

    // current weapon out
    Meta(Disable)
    WeaponType m_prevWeapon;

    // position of camera
    Meta(Disable)
    ursine::ecs::Transform* m_cameraHandle;

    // position of arm
    Meta(Disable)
    ursine::ecs::Transform* m_armHandle;

    // what archetype to load
    Meta(Disable)
    WeaponSlotInfo m_inventory[ 5 ];

    // was swap key triggered
    Meta(Disable)
    bool m_swap;

    Meta(Disable)
    bool m_newWeapon;

    Meta(Disable)
    bool m_init;


private:
    void TriggerSwapWeapons(EVENT_HANDLER(game::SWAP_COMMAND));

} Meta(Enable, DisplayName("Inventory"));


//struct Inventory2 : ursine::ecs::Component
//{
//    NATIVE_COMPONENT;
//
//public:
//    static const int INVENTORY_COUNT = 2;
//
//    Inventory2(void);
//
//    void OnInitialize(void) override;
//    void Init(void);
//
//    EditorField(
//        WeaponType StartWeapon,
//        GetStartWeapon,
//        SetStartWeapon
//        );
//
//    EditorField(
//        std::string SecondaryWeapon,
//        GetSecondaryWeapon,
//        SetSecondaryWeapon
//        );
//
//    EditorField(
//        std::string PrimaryWeapon,
//        GetPrimaryWeapon,
//        SetPrimaryWeapon
//        );
//
//
//    WeaponType GetStartWeapon(void) const;
//    void SetStartWeapon(const WeaponType slot);
//
//    const std::string& GetSecondaryWeapon(void) const;
//    void SetSecondaryWeapon(const std::string& archetype);
//
//    const std::string& GetPrimaryWeapon(void) const;
//    void SetPrimaryWeapon(const std::string& archetype);
//
//    void SetNewWeapon(const WeaponType type, const std::string& weaponToLoad, int ammo, int clip);
//
//
//    ///////////////////
//    ////  Members  ////
//    ///////////////////
//
//    // current weapon out
//    WeaponType m_currWeapon;
//
//    // current weapon out
//    Meta(Disable)
//        WeaponType m_prevWeapon;
//
//    // position of camera
//    Meta(Disable)
//        ursine::ecs::Transform* m_cameraHandle;
//
//    // position of arm
//    Meta(Disable)
//        ursine::ecs::Transform* m_armHandle;
//
//    // what archetype to load
//    Meta(Disable)
//        WeaponSlotInfo m_inventory[ 3 ];
//
//    // was swap key triggered
//    Meta(Disable)
//        bool m_swap;
//
//    Meta(Disable)
//        bool m_newWeapon;
//
//    Meta(Disable)
//        bool m_init;
//
//
//private:
//    void TriggerSwapWeapons(EVENT_HANDLER(game::SWAP_COMMAND));
//
//} Meta(Enable, DisplayName("Inventory2"));
//

