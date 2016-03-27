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
    HEALTH_ZERO
};

struct HealthEventArgs : ursine::EventArgs
{
    float health;
    float percentage;

    HealthEventArgs(float health, float percentage)
        : health( health )
        , percentage( percentage ) { }
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

    Meta(Enable)
    Health(void);
    ~Health(void);

    HealthType GetHealthType(void) const;
    void SetHealthType(HealthType type);

    float GetHealth(void) const;
    void SetHealth(float health);
    float GetMaxHealth(void) const;

    const ursine::resources::ResourceReference &GetArchetypeOnDeath(void) const;
    void SetArchetypeOnDeath(const ursine::resources::ResourceReference &objToSpawn);

    bool GetDeleteOnZeroHealth(void) const;
    void SetDeleteOnZeroHealth(bool flag);

    bool GetSpawnOnDeath(void) const;
    void SetSpawnOnDeath(bool state);

    bool GetInvulnerable(void) const;
    void SetInvulnerable(bool invulnerable);

    void DealDamage(float damage);
    void DealDamage(const ursine::SVec3& contactPoint, float damage, bool crit);

    bool CanDamage(DamageOnCollide *damage) const;
    bool CanDamage(AbstractHitscanWeapon *weapon) const;

private:
    void OnInitialize(void) override;
    void sendDamageTextEvent(const ursine::SVec3& contact, float damage, bool crit);

    void OnDeath(EVENT_HANDLER(ursine::ecs::Entity));

    HealthType m_type;

    float m_health;
    float m_maxHealth;

    ursine::resources::ResourceReference m_objToSpawn;

    bool m_deleteOnZero;
    bool m_spawnOnDeath;

    // A flag letting us know if we're dead or not.
    // This solves the problem of dealing damage after it's already dead.
    bool m_dead;

    // Flag letting us know if we're invulnerable
    bool m_invulnerable;

} Meta(Enable, WhiteListMethods, DisplayName( "Health" ));
