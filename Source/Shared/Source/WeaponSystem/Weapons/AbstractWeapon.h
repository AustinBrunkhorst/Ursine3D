/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AbstractComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "Randomizer.h"
#include "DamageOnCollideComponent.h"

#include <ArchetypeData.h>

#define UNLIMITED_AMMO  MAXINT
#define UNLIMITED_CLIP  MAXINT

// Weapon Statuses
#define MUST_RELOAD         0b00000001
#define RELOAD_IN_PROCESS   0b00000010
#define FIRE_TIMER_SET      0b00000100
#define TRIGGER_NOT_PULLED  0b00001000
#define CAN_FIRE            0b00010000

// Reload Info
#define RELOAD_SUCCESS 0b00000001
#define NO_AMMO        0b00000010
#define CLIP_FULL      0b00000100

#define MAX_AMMO false
#define AMMO_USED true

#define AbstractWeaponFields( )            \
    EditorField(                           \
        bool SemiAutomatic,                \
        GetSemiAutomatic,                  \
        SetSemiAutomatic                   \
    );                                     \
                                           \
    EditorField(                           \
        float FireDelay,                   \
        GetFireRate,                       \
        SetFireRate                        \
    );                                     \
                                           \
    EditorField(                           \
        float ReloadTime,                  \
        GetReloadTime,                     \
        SetReloadTime                      \
    );                                     \
                                           \
    EditorField(                           \
        float RecoilAngle,                 \
        GetRecoilAngle,                    \
        SetRecoilAngle                     \
    );                                     \
                                           \
    EditorField(                           \
        float Range,                       \
        GetMaxRange,                       \
        SetMaxRange                        \
    );                                     \
                                           \
    EditorField(                           \
        float Accuracy,                    \
        GetAccuracy,                       \
        SetAccuracy                        \
    );                                     \
                                           \
    EditorField(                           \
        float SpreadFactor,                \
        GetSpreadFactor,                   \
        SetSpreadFactor                    \
    );                                     \
                                           \
    EditorField(                           \
        int MaxAmmoCount,                  \
        GetMaxAmmoCount,                   \
        SetMaxAmmoCount                    \
    );                                     \
                                           \
    EditorField(                           \
        int ClipSize,                      \
        GetClipSize,                       \
        SetClipSize                        \
    );                                     \
                                           \
    EditorField(                           \
        int ProjectileFireCount,           \
        GetProjFireCount,                  \
        SetProjFireCount                   \
    );                                     \
                                           \
    EditorField(                           \
        WeaponType WeaponTier,             \
        GetWeaponType,                     \
        SetWeaponType                      \
    );                                     \
                                           \
    EditorResourceField(                   \
        ursine::resources::ArchetypeData,  \
        MuzzleParticle,                    \
        GetFireParticle,                   \
        SetFireParticle                    \
    );                                     


#define AbstractWeaponConnect( Obj )                            \
    GetOwner( )->Listener( this )                               \
        .On( game::ACTIVATE_WEAPON, &Obj::ActivateWeapon )      \
        .On( game::DETACH_WEAPON, &Obj::DetachWeapon )          \
        .On( game::DEACTIVATE_WEAPON, &Obj::DeactivateWeapon) ;

#define AbstractWeaponDisconnect( Obj )                          \
    GetOwner( )->Listener( this )                                \
        .Off( game::ACTIVATE_WEAPON, &Obj::ActivateWeapon )      \
        .Off( game::DETACH_WEAPON, &Obj::DetachWeapon )          \
        .Off( game::DEACTIVATE_WEAPON, &Obj::DeactivateWeapon );

namespace ursine
{
    namespace ecs
    {
        class Transform;
        class Animator;
    }
}

enum WeaponType
{
    LAST_STAND,
    SECONDARY_WEAPON,
    PRIMARY_WEAPON,
    MELEE_WEAPON,
    GOD_WEAPON
} Meta(Enable);

struct AbstractWeapon
{
public:
    AbstractWeapon( );
    virtual ~AbstractWeapon( void );

    void Initialize(const ursine::ecs::EntityHandle &owner);

    /////////////////////////////
    //// Weapon Setup Logic  ////
    /////////////////////////////

    // Activate Weapon for use
    void ActivateWeapon(
        const ursine::ecs::EntityHandle &owner, 
        const ursine::ecs::EntityHandle &whatToConnect, 
        ursine::ecs::Transform* camHandle, int ammo, int clip
    );

    // Detatch weapon from parent and turn into interactable
    void DetachWeapon(
        const ursine::ecs::EntityHandle &owner, 
        const ursine::ecs::EntityHandle &whatToConnect
    );

    // Deactivate Weapon for use
    void DeactivateWeapon(
        const ursine::ecs::EntityHandle &whatToDisconnect, 
        int& saveAmmo, int& saveClip
    );

    /////////////////////////////
    ////  Weapon Fire Logic  ////
    /////////////////////////////
    virtual int FireLogic(void);
    virtual int CanFire(void) const;


    ///////////////////////////////
    ////  Gettors and Settors  ////
    ///////////////////////////////
    float GetFireRate(void) const;
    void SetFireRate(float rate);

    float GetReloadTime(void) const;
    void SetReloadTime(float time);

    float GetRecoilAngle(void) const;
    void SetRecoilAngle(float angle);

    float GetMaxRange(void) const;
    void SetMaxRange(float range);

    float GetAccuracy(void) const;
    void SetAccuracy(float accuracy);

    float GetSpreadFactor(void) const;
    void SetSpreadFactor(float spread);

    int GetAmmoCount(void) const;
    void SetAmmoCount(int ammo);

    int GetMaxAmmoCount(void) const;
    void SetMaxAmmoCount(int maxAmmo);

    int GetClipCount(void) const;
    void SetClipCount(int count);

    int GetClipSize(void) const;
    void SetClipSize(int size);

    int GetProjFireCount(void) const;
    void SetProjFireCount(int count);

    WeaponType GetWeaponType(void) const;
    void SetWeaponType(WeaponType type);

    const ursine::resources::ResourceReference &GetFireParticle(void) const;
    void SetFireParticle(const ursine::resources::ResourceReference &archetype);

    bool GetSemiAutomatic(void) const;
    void SetSemiAutomatic(bool semi);

    bool GetTriggerPulled(void) const;

    void ConnectTrigger(const ursine::ecs::EntityHandle &obj);

protected:
    friend class WeaponSystemUtils;
    friend class BaseWeaponSystem;
    friend class HitscanWeaponSystem;

    // who is my owner
    ursine::ecs::EntityHandle m_owner;

    // Rate at which bullets can be fired
    float m_fireRate;

    // Timer To keep track of when weapon can fire next
    float m_fireTimer;

    // How long does reload take
    float m_reloadTime;

    // Timer to keep track of reloading
    float m_reloadTimer;

    // What angle do you want recoil to be applied at?
    float m_recoilAngle;

    // How far is weapons range
    float m_maxRange;

    // what is accuracy of gun
    float m_accuracy;

    // How much ammo does weapon have
    int m_ammoCount;

    // Max amount of ammo weapon may hold
    int m_maxAmmoCount;

    // How much ammo is in clip
    int m_clipCount;

    // Max amount that clip can hold
    int m_clipSize;

    // count of how many projectiles weapon fires per shot
    int m_projFireCount;

    // weapon type 
    WeaponType m_weaponType;

    // Camera Handle for shooting
    ursine::ecs::Transform* m_camHandle;

    // fire position Handle for shooting
    ursine::ecs::Transform* m_firePosHandle;

    // fire position Handle for shooting
    ursine::ecs::Animator* m_animatorHandle;

    // spread to apply to shot
    ursine::Randomizer m_spread;

    // Particle to spawn at tip of gun when shot
    ursine::resources::ResourceReference m_fireParticle;

    // is weapon a semi-automatic
    bool m_semiAutomatic;

    // Is trigger being pulled
    bool m_triggerPulled;

    // can i shoot?
    bool m_active;

    // Weapons trigger is being pulled
    void TriggerPulled(EVENT_HANDLER(game::FIRE_START));

    // Weapon's trigger was released
    void TriggerReleased(EVENT_HANDLER(game::FIRE_END));

    bool AddAmmo(const int ammo);

    void ActivateWeapon(EVENT_HANDLER(game::ACTIVATE_WEAPON));
    void DetachWeapon(EVENT_HANDLER(game::DETACH_WEAPON));
    void DeactivateWeapon(EVENT_HANDLER(game::DEACTIVATE_WEAPON));

    void PickUpAmmo(EVENT_HANDLER(ursine::ecs::PICKUP_AMMO));

    //***************************************//
    //  MUST IMPLEMENT, so base class can    //
    //  remove component from its owner      //
    //***************************************//
    virtual void RemoveMySelf(void) = 0;

};

#define AbstractWeaponInit( Obj, owner )   AbstractWeapon::Initialize( owner );  
