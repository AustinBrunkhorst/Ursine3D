/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BaseWeaponComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/


#pragma once

#include <Component.h>

// Weapon Statuses
#define MUST_RELOAD         0b00000001
#define RELOAD_IN_PROCESS   0b00000010
#define FIRE_TIMER_SET      0b00000100
#define TRIGGER_NOT_PULLED  0b00001000
#define CAN_FIRE            0b00010000

// Reload Info
#define RELOAD_SUCCES 0b00000000
#define NO_AMMO       0b00000001
#define CLIP_FULL     0b00000010


enum AOEType
{
    None = 0,
    Cone = 2,
    Cube = 4,
    Cylinder = 8,
    Sphere = 16
    
} Meta(Enable);

enum ShotType
{
    Single_Shot = 0,
    Two_Burst = 2,
    Triple_Burst = 4,
    Quad_Burst = 8,
    Full_Clip = 16,
    Random_Burst = 32
} Meta(Enable);

class BaseWeapon : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    /*EditorField(
        float Damage,
        GetDamage,
        SetDamage
        );

    EditorField(
        float CritModifier,
        GetCritModifier,
        SetCritModifier
        );

    EditorField(
        float CritChance,
        GetCritChance,
        SetCritChance
        );*/

    EditorField(
        float FireRate,
        GetFireRate,
        SetFireRate
        );

    EditorField(
        float ReloadTime,
        GetReloadTime,
        SetReloadTime
        );

    EditorField(
        float RecoilAngle,
        GetRecoilAngle,
        SetRecoilAngle
        );

    EditorField(
        float MaxRange,
        GetMaxRange,
        SetMaxRange
        );

    EditorField(
        int MaxAmmoCount,
        GetMaxAmmoCount,
        SetMaxAmmoCount
        );

    EditorField(
        int ClipSize,
        GetClipSize,
        SetClipSize
        );

    EditorField(
        ShotType Shot_Type,
        GetShotType,
        SetShotType
        );

    EditorField(
        AOEType AOE_Type,
        GetAOEType,
        SetAOEType
        );

    EditorField(
        std::string ArchetypeToShoot,
        GetArchetypeToShoot,
        SetArchetypeToShoot
        );


    BaseWeapon(void);
    ~BaseWeapon(void);

    void OnInitialize(void) override;

    /*float GetDamage(void);
    void SetDamage(const float damage);

    float GetCritModifier(void);
    void SetCritModifier(const float modifier);

    float GetCritChance(void);
    void SetCritChance(const float critChance); */

    ///////////////////////////////
    ////  Gettors and Settors  ////
    ///////////////////////////////

    float GetFireRate(void) const;
    void SetFireRate(const float rate);

    float GetReloadTime( void ) const;
    void SetReloadTime( const float time );

    float GetRecoilAngle(void) const;
    void SetRecoilAngle(const float angle);

    float GetMaxRange(void) const;
    void SetMaxRange(const float range);

    int GetAmmoCount(void) const;
    void SetAmmoCount(const int ammo);

    int GetMaxAmmoCount(void) const;
    void SetMaxAmmoCount(const int maxAmmo);

    int GetClipCount(void) const;
    void SetClipCount(const int count);

    int GetClipSize(void) const;
    void SetClipSize(const int size);

    ShotType GetShotType(void) const;
    void SetShotType(const ShotType type);

    AOEType GetAOEType(void) const;
    void SetAOEType(const AOEType type);

    const std::string& GetArchetypeToShoot(void) const;
    void SetArchetypeToShoot(const char* archetype);
    void SetArchetypeToShoot(const std::string& archetype);

    bool GetTriggerPulled(void) const;

    //////////////////////
    ////  Fire Logic  ////
    //////////////////////

    int CanFire(void) const;
    int Fire(void);

    ////////////////////////
    ////  Weapon Logic  ////
    ////////////////////////

    void DecrementFireTimer(const float dt);
    void DecrementReloadTimer( const float dt );

    // Add ammo to current ammo count
    bool AddAmmo(const int ammo);

    // Reload weapon
    int Reload(void);
  

private:

   /* 
        Ranged weapons that are the standard in battle.Stats include damage (d),
        critical damage (d), damage over time, range, area - of - effect,
        projectile speed, rate of fire, ammo per shot, magazine size,
        reload speed, recoil, aiming FOV, base movement modifier,
        and aiming movement modifier.
   */

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

    // How much ammo does weapon have
    int m_ammoCount;

    // Max amount of ammo weapon may hold
    int m_maxAmmoCount;

    // How much ammo is in clip
    int m_clipCount;

    // Max amount that clip can hold
    int m_clipSize;

    // Num of bullets to be shot
    ShotType m_shotType;
    
    // Area of affect to be applied to shot
    AOEType m_aoeType;

    // Archetype weapon should fire
    std::string m_archetypeToShoot;

    // Is trigger being pulled
    bool m_triggerPulled;


    // Weapons trigger is being pulled
    Meta(Disable)
    void TriggerPulled(EVENT_HANDLER(gameEvent::FIRE_START));

    // Weapon's trigger was released
    Meta(Disable)
    void TriggerReleased(EVENT_HANDLER(gameEvent::FIRE_END));

    // Tries to remove the number of rounds specified from the clip
    //   and returns the actual number of rounds removed
    int RemoveRoundsFromClip(int roundCount);

    // Is weapon out of ammo
    bool OutofAmmo( void );

    // Is weapon's clip full
    bool ClipFull( void );

} Meta(Enable, DisplayName("Base Weapon"));

