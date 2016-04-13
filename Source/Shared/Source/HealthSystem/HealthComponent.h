/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** HealthComponent.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include <ArchetypeData.h>

namespace ursine
{
    namespace physics
    {
        class Contact;
    }
}

class DamageOnCollide;
struct AbstractHitscanWeapon;

enum HealthEvents
{
    HEALTH_DAMAGE_TAKEN,
    HEALTH_DAMAGED,
    HEALTH_REVIVE,
    HEALTH_ZERO
};

struct HealthEventArgs : ursine::EventArgs
{
    float health;
    float percentage;
    ursine::ecs::EntityHandle damager;

    HealthEventArgs(float health, float percentage, ursine::ecs::EntityHandle damager)
        : health( health )
        , percentage( percentage )
        , damager( damager ) { }
};

enum HealthType
{
    ENEMY_HEALTH,
    PLAYER_HEALTH
} Meta(Enable);

class Health 
    : public ursine::ecs::Component
    , public ursine::EventDispatcher<HealthEvents>
{
    NATIVE_COMPONENT;

public:
    EditorField(
        HealthType healthType,
        GetHealthType,
        SetHealthType
    );

    EditorField(
        float EntityHealth,
        GetHealth,
        SetHealth
    );

    EditorField(
        float MaxHealth,
        GetMaxHealth,
        SetMaxHealth
        );

    EditorResourceField(
        ursine::resources::ArchetypeData,
        archetypeToSpawnOnDeath,
        GetArchetypeOnDeath,
        SetArchetypeOnDeath
    );

    EditorField(
        bool deleteOnZeroHealth,
        GetDeleteOnZeroHealth,
        SetDeleteOnZeroHealth
    );

    EditorField(
        bool SpawnOnDeath,
        GetSpawnOnDeath,
        SetSpawnOnDeath
    );

    EditorField(
        bool invulnerable,
        GetInvulnerable,
        SetInvulnerable
    );

    EditorField(
        bool hasShield,
        GetHasShield,
        SetHasShield
    );

    EditorField(
        float ShieldHealth,
        GetShieldHealth,
        SetShieldHealth
    );

    EditorField(
        float MaxShieldHealth,
        GetMaxShieldHealth,
        SetMaxShieldHealth
    );

    Meta(Enable)
    Health(void);

    HealthType GetHealthType(void) const;
    void SetHealthType(HealthType type);

    float GetHealth(void) const;
    void SetHealth(float health);

    float GetMaxHealth(void) const;
    void SetMaxHealth(float health);

    const ursine::resources::ResourceReference &GetArchetypeOnDeath(void) const;
    void SetArchetypeOnDeath(const ursine::resources::ResourceReference &objToSpawn);

    bool GetDeleteOnZeroHealth(void) const;
    void SetDeleteOnZeroHealth(bool flag);

    bool GetSpawnOnDeath(void) const;
    void SetSpawnOnDeath(bool state);

    bool GetInvulnerable(void) const;
    void SetInvulnerable(bool invulnerable);

    bool GetHasShield(void) const;
    void SetHasShield(bool toggle);

    float GetShieldHealth(void) const;
    void SetShieldHealth(float shield);

    float GetMaxShieldHealth(void) const;
    void SetMaxShieldHealth(float shield);

    void AddHealth(float healthToAdd);
    void AddShieldHealth(float healthToAdd);

    void DealDamage(const ursine::ecs::EntityHandle &damager, float damage);
    void DealDamage(const ursine::ecs::EntityHandle &damager,
                    const ursine::SVec3& contactPoint, float damage, bool crit);

    bool CanDamage(DamageOnCollide *damage) const;
    bool CanDamage(AbstractHitscanWeapon *weapon) const;

private:
    void OnInitialize(void) override;
    void sendDamageTextEvent(const ursine::SVec3& contact, const ursine::ecs::EntityHandle &damager, float damage, bool crit);

    void onDeath(EVENT_HANDLER(ursine::ecs::Entity));

    void onRevive(EVENT_HANDLER(ursine::ecs::Entity));

    void sendDamageUIEvent(const ursine::ecs::EntityHandle &damager, float damage);

    HealthType m_type;

    float m_health;
    float m_maxHealth;

    float m_shield;
    float m_maxShield;

    ursine::resources::ResourceReference m_objToSpawn;

    bool m_deleteOnZero;
    bool m_spawnOnDeath;

    // Flag letting us know if we're invulnerable
    bool m_invulnerable;

    bool m_hasShield;

} Meta(Enable, WhiteListMethods, DisplayName( "Health" ));
