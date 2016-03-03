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
#include <RigidbodyComponent.h>

namespace ursine
{
    namespace physics
    {
        class Contact;
    } // physics namespace
} // ursine namespace

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

class Health 
    : public ursine::ecs::Component
    , public ursine::EventDispatcher<HealthEvents>
{
    NATIVE_COMPONENT;

public:
    EditorField(
        float EntityHealth,
        GetHealth,
        SetHealth
    );

    EditorField(
        std::string ArchetypeToSpawnOnDeath,
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

    Meta(Enable)
    Health(void);
    ~Health(void);

    float GetHealth(void) const;
    void SetHealth(const float health);
    float GetMaxHealth(void) const;

    const std::string& GetArchetypeOnDeath(void) const;
    void SetArchetypeOnDeath(const std::string& objToSpawn);

    bool GetDeleteOnZeroHealth(void) const;
    void SetDeleteOnZeroHealth(bool flag);

    bool GetSpawnOnDeath(void) const;
    void SetSpawnOnDeath(const bool state);

    void DealDamage(const float damage);
    void DealDamage(const ursine::SVec3& contactPoint, float damage, bool crit);

private:
    void OnInitialize(void) override;
    void sendDamageTextEvent(const ursine::SVec3& contact, float damage, bool crit);

    void OnDeath(EVENT_HANDLER(ursine::ecs::ENTITY_REMOVED));

    float m_health;
    float m_maxHealth;

    std::string m_objToSpawn;

    bool m_deleteOnZero;
    bool m_spawnOnDeath;

} Meta(Enable, WhiteListMethods, DisplayName( "Health" ));
