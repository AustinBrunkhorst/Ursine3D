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


#define AbstractWeaponFields( )         \
    EditorField(                        \
        float Damage,                   \
        GetDamageToApply,               \
        SetDamageToApply                \
        );                              \
                                        \
    EditorField(                        \
        float CritModifier,             \
        GetCritModifier,                \
        SetCritModifier                 \
        );                              \
                                        \
    EditorField(                        \
        float DamageInterval,           \
        GetDamageInterval,              \
        SetDamageInterval               \
        );                              \
                                        \
    EditorField(                        \
        bool DelBulletOnCollision,      \
        GetDeleteOnCollision,           \
        SetDeleteOnCollision            \
        );                              \
                                        \
    EditorField(                        \
        float ProjectileSpeed,          \
        GetProjSpeed,                   \
        SetProjSpeed                    \
        );                              \
                                        \
    EditorField(                        \
        float FireRate,                 \
        GetFireRate,                    \
        SetFireRate                     \
        );                              \
                                        \
    EditorField(                        \
        float ReloadTime,               \
        GetReloadTime,                  \
        SetReloadTime                   \
        );                              \
                                        \
    EditorField(                        \
        float RecoilAngle,              \
        GetRecoilAngle,                 \
        SetRecoilAngle                  \
        );                              \
                                        \
    EditorField(                        \
        float MaxRange,                 \
        GetMaxRange,                    \
        SetMaxRange                     \
        );                              \
                                        \
    EditorField(                        \
        float Accuracy,                 \
        GetAccuracy,                    \
        SetAccuracy                     \
        );                              \
                                        \
    EditorField(                        \
        float SpreadFactor,             \
        GetSpreadFactor,                \
        SetSpreadFactor                 \
        );                              \
                                        \
    EditorField(                        \
        int MaxAmmoCount,               \
        GetMaxAmmoCount,                \
        SetMaxAmmoCount                 \
        );                              \
                                        \
    EditorField(                        \
        int ClipSize,                   \
        GetClipSize,                    \
        SetClipSize                     \
        );                              \
                                        \
    EditorField(                        \
        int FireCount,                  \
        GetProjFireCount,               \
        SetProjFireCount                \
        );                              \
                                        \
    EditorField(                        \
        WeaponType WeaponTier,          \
        GetWeaponType,                  \
        SetWeaponType                   \
        );                              \
                                        \
    EditorField(                        \
        ursine::SVec3 SpawnOffset,      \
        GetSpawnOffset,                 \
        SetSpawnOffset                  \
        );                              \
                                        \
    EditorField(                        \
        std::string ArchetypeToShoot,   \
        GetArchetypeToShoot,            \
        SetArchetypeToShoot             \
        );
  
#define AbstractWeaponConnect( Obj )                            \
    GetOwner( )->Listener( this )                               \
        .On(game::ACTIVATE_WEAPON, &Obj::ActivateWeapon)        \
        .On(game::DETACH_WEAPON, &Obj::DetachWeapon)            \
        .On(game::DEACTIVATE_WEAPON, &Obj::DeactivateWeapon);

#define AbstractWeaponDisconnect( Obj )                          \
    GetOwner( )->Listener( this )                                \
        .Off(game::ACTIVATE_WEAPON, &Obj::ActivateWeapon)        \
        .Off(game::DETACH_WEAPON, &Obj::DetachWeapon)            \
        .Off(game::DEACTIVATE_WEAPON, &Obj::DeactivateWeapon);


namespace ursine
{
    namespace ecs
    {
        class Animator;
    } // ecs namespace
} // ursine namespace


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
    // damage to apply when triggered
    float m_damageToApply;

    // scalar to apply to damage
    float m_critModifier;

    // time to wait until apply damage again
    //   (keeps from applying damage each frame)
    float m_damageInterval;

    // does projectile die on first collision
    bool m_deleteOnCollision;

    // projectile speed
    float m_projSpeed;

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

    // what is the weapons spread factor for shooting
    float m_spreadFactor;

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

    // what offset to add on activation
    ursine::SVec3 m_spawnOffset;

    // Camera Handle for shooting
    Meta(Disable)
    ursine::ecs::Component::Handle<ursine::ecs::Transform> m_camHandle;

    // fire position Handle for shooting
    Meta(Disable)
    ursine::ecs::Component::Handle<ursine::ecs::Transform> m_firePosHandle;

    // fire position Handle for shooting
    Meta(Disable)
    ursine::ecs::Component::Handle<ursine::ecs::Animator> m_animatorHandle;

    // Archetype weapon should fire
    std::string m_archetypeToShoot;

    // Is trigger being pulled
    Meta(Disable)
    bool m_triggerPulled;

    // can i shoot?
    Meta(Disable)
    bool m_active;


    AbstractWeapon( void );
    virtual ~AbstractWeapon( void );

    void Initialize(void);

    /////////////////////////////
    ////  Weapon Fire Logic  ////
    /////////////////////////////
    virtual int FireLogic(void);
    virtual int CanFire(void) const;


    ///////////////////////////////
    ////  Gettors and Settors  ////
    ///////////////////////////////
    float GetDamageToApply(void) const;
    void  SetDamageToApply(const float damage);

    float GetCritModifier(void) const;
    void SetCritModifier(const float modifier);

    float GetDamageInterval(void) const;
    void SetDamageInterval(const float damageInterval);

    bool GetDeleteOnCollision(void) const;
    void SetDeleteOnCollision(const bool state);

    float GetProjSpeed(void) const;
    void SetProjSpeed(const float speed);

    float GetFireRate(void) const;
    void SetFireRate(const float rate);

    float GetReloadTime(void) const;
    void SetReloadTime(const float time);

    float GetRecoilAngle(void) const;
    void SetRecoilAngle(const float angle);

    float GetMaxRange(void) const;
    void SetMaxRange(const float range);

    float GetAccuracy(void) const;
    void SetAccuracy(const float accuracy);

    float GetSpreadFactor(void) const;
    void SetSpreadFactor(const float spread);

    int GetAmmoCount(void) const;
    void SetAmmoCount(const int ammo);

    int GetMaxAmmoCount(void) const;
    void SetMaxAmmoCount(const int maxAmmo);

    int GetClipCount(void) const;
    void SetClipCount(const int count);

    int GetClipSize(void) const;
    void SetClipSize(const int size);

    int GetProjFireCount(void) const;
    void SetProjFireCount(const int count);

    WeaponType GetWeaponType(void) const;
    void SetWeaponType(const WeaponType type);

    ursine::SVec3 GetSpawnOffset(void) const;
    void SetSpawnOffset(const ursine::SVec3& offset);

    const std::string& GetArchetypeToShoot(void) const;
    void SetArchetypeToShoot(const char* archetype);
    void SetArchetypeToShoot(const std::string& archetype);

    bool GetTriggerPulled(void) const;

protected:

    // Weapons trigger is being pulled
    void TriggerPulled(EVENT_HANDLER(game::FIRE_START));

    // Weapon's trigger was released
    void TriggerReleased(EVENT_HANDLER(game::FIRE_END));

    // Activate Weapon for use
    void ActivateWeapon(EVENT_HANDLER(game::ACTIVATE_WEAPON));

    // Detatch weapon from parent and turn into interactable
    void DetachWeapon(EVENT_HANDLER(game::DETACH_WEAPON));

    // Deactivate Weapon for use
    void DeactivateWeapon(EVENT_HANDLER(game::DEACTIVATE_WEAPON));


    bool AddAmmo(const int ammo);

    void PickUpAmmo(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED));


    //***************************************//
    //  MUST IMPLEMENT, so base class can    //
    //  remove component from its owner      //
    //***************************************//
    virtual void RemoveMySelf(void) = 0;

};


#define AbstractWeaponInit( Obj )   AbstractWeapon::Initialize( );    \
                                    AbstractWeaponConnect(Obj);









