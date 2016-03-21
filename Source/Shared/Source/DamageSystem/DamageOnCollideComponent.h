/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** DamageOnCollideComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include <ArchetypeData.h>

#include "HealthComponent.h"

namespace ursine
{
    namespace physics
    {
        struct RaycastOutput;
    }
}

struct CritSpot;

enum DamageType
{
    DAMAGE_ENEMY = ENEMY_HEALTH,
    DAMAGE_PLAYER = PLAYER_HEALTH,
    DAMAGE_ALL
} Meta(Enable);

class DamageOnCollide : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorField(
        DamageType damageType,
        GetDamageType,
        SetDamageType
    );

    EditorField(
        float DamageToApply,
        GetDamageToApply,
        SetDamageToApply
    );

    EditorField(
        float CritModifier,
        GetCritModifier,
        SetCritModifier
    );

    EditorResourceField(
        ursine::resources::ArchetypeData,
        archetypeOnDeath,
        GetArchetypeOnDeath,
        SetArchetypeOnDeath
    );

    EditorResourceField(
        ursine::resources::ArchetypeData,
        archetypeOnHit,
        GetArchetypeOnHit,
        SetArchetypeOnHit
    );

    EditorField(
        float DamageInterval,
        GetDamageInterval,
        SetDamageInterval
    );

    EditorField(
        bool DeleteOnCollision,
        GetDeleteOnCollision,
        SetDeleteOnCollision
    );

    EditorField(
        bool SpawnOnDeath,
        GetSpawnOnDeath,
        SetSpawnOnDeath
    );

    EditorField(
        bool SpawnOnHit,
        GetSpawnOnHit,
        SetSpawnOnHit
    );

    EditorField(
        bool listenToChildren,
        GetListenToChildren,
        SetListenToChildren
    );

    DamageOnCollide(void);
    ~DamageOnCollide(void);

    void OnInitialize(void) override;

    DamageType GetDamageType(void) const;
    void SetDamageType(DamageType type);

    float GetDamageToApply(void) const;
    void SetDamageToApply(float damage);

    float GetCritModifier(void) const;
    void SetCritModifier(float modifier);

    float GetDamageInterval(void) const;
    void SetDamageInterval(float damageInterval);

    const ursine::resources::ResourceReference &GetArchetypeOnDeath(void) const;
    void SetArchetypeOnDeath(const ursine::resources::ResourceReference &objToSpawn);

    const ursine::resources::ResourceReference &GetArchetypeOnHit(void) const;
    void SetArchetypeOnHit(const ursine::resources::ResourceReference &objToSpawn);

    bool GetDeleteOnCollision(void) const;
    void SetDeleteOnCollision(bool state);

    bool GetSpawnOnDeath(void) const;
    void SetSpawnOnDeath(bool state);

    bool GetSpawnOnHit(void) const;
    void SetSpawnOnHit(bool state);

    bool GetListenToChildren(void) const;
    void SetListenToChildren(bool flag);

    void DecrementDamageIntervalTimes(float dt);

private:
    // The type of damage that is being dealth (player, enemy, all)
    DamageType m_type;

    // damage to apply when triggered
    float m_damageToApply;

    // scalar to apply to damage
    float m_critModifier;

    // time to wait until apply damage again
    //   (keeps from applying damage each frame)
    float m_damageInterval;

    // archetype that will be spawned at the point that the 
    //   object dies
    ursine::resources::ResourceReference m_objToSpawn;

    // archetype that will be spawned at the point that the 
    //   object deals damage to another object
    ursine::resources::ResourceReference m_objToSpawnOnHit;

    // does projectile die on first collision
    bool m_deleteOnCollision;

    // was projectile already deleted this frame
    //   (make sure damage only applied to one collision if delete on collision is set)
    bool m_deleted;

    bool m_spawnOnHit;
    bool m_spawnOnDeath;

    bool m_listenToChildren;
    bool m_serialized;

    // map of all objects hit
    std::unordered_map<ursine::ecs::EntityHandle, float> m_damageTimeMap;

    void onDeath(EVENT_HANDLER(ursine::ecs::Entity));
    void onCollide(EVENT_HANDLER(ursine::ecs::Entity));
    void onHierarchySerialized(EVENT_HANDLER(ursine::ecs::Entity));

    void getSpawnLocation(
        const ursine::ecs::EntityHandle &other, 
        ursine::physics::RaycastOutput& rayout, 
        ursine::SVec3& posToSet
    );

    // spawn particle at collision point and parent to player
    void spawnCollisionParticle(const ursine::ecs::EntityHandle &other, bool crit);

    void deleteOnCollision(void);

    void applyDamage(
        const ursine::ecs::EntityHandle &obj, 
        const ursine::SVec3& contact, 
        float damage, bool crit
    );

    void connectToChildrenCollisionEvents(
        bool connect, 
        const std::vector<ursine::ecs::EntityID> *children
    );

} Meta (Enable, DisplayName( "DamageOnCollide" ));
