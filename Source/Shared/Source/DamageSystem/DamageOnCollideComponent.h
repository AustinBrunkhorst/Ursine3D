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

namespace ursine
{
    namespace physics
    {
        struct RaycastOutput;
    } // physics namespace
} // ursine namespace

class Health;
struct CritSpot;

class DamageOnCollide : public ursine::ecs::Component
{
    NATIVE_COMPONENT;
public:

    ////////////////////////////////////////////////////////////////////
    // Expose data to editor
    ////////////////////////////////////////////////////////////////////
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

    EditorField(
        std::string ArchetypeOnDeath,
        GetArchetypeOnDeath,
        SetArchetypeOnDeath
    );

    EditorField(
        std::string ArchetypeOnHit,
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

    DamageOnCollide(void);
    ~DamageOnCollide(void);

    void OnInitialize(void) override;

    float GetDamageToApply(void) const;
    void SetDamageToApply(float damage);

    float GetCritModifier(void) const;
    void SetCritModifier(float modifier);

    float GetDamageInterval(void) const;
    void SetDamageInterval(float damageInterval);

    const std::string& GetArchetypeOnDeath(void) const;
    void SetArchetypeOnDeath(const std::string& objToSpawn);

    const std::string& GetArchetypeOnHit(void) const;
    void SetArchetypeOnHit(const std::string& objToSpawn);

    bool GetDeleteOnCollision(void) const;
    void SetDeleteOnCollision(bool state);

    bool GetSpawnOnDeath(void) const;
    void SetSpawnOnDeath(bool state);

    bool GetSpawnOnHit(void) const;
    void SetSpawnOnHit(bool state);

    void DecrementDamageIntervalTimes(float dt);

private:

    // damage to apply when triggered
    float m_damageToApply;

    // scalar to apply to damage
    float m_critModifier;

    // time to wait until apply damage again
    //   (keeps from applying damage each frame)
    float m_damageInterval;

    // archetype that will be spawned at the point that the 
    //   object dies
    std::string m_objToSpawn;

    // archetype that will be spawned at the point that the 
    //   object deals damage to another object
    std::string m_objToSpawnOnHit;

    // does projectile die on first collision
    bool m_deleteOnCollision;

    // was projectile already deleted this frame
    //   (make sure damage only applied to one collision if delete on collision is set)
    bool m_deleted;

    bool m_spawnOnHit;
    bool m_spawnOnDeath;

    // map of all objects hit
    std::unordered_map<ursine::ecs::EntityUniqueID, float> m_damageTimeMap;

    void onDeath(EVENT_HANDLER(ursine::ecs::ENTITY_REMOVED));
    void onCollide(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED));

    void getSpawnLocation(ursine::ecs::Entity* other, ursine::physics::RaycastOutput& rayout, ursine::SVec3& posToSet);

    // spawn particle at collision point and parent to player
    void spawnCollisionParticle(ursine::ecs::Entity* other, bool crit);

    void deleteOnCollision(void);

    void applyDamage(ursine::ecs::Entity* obj, const ursine::SVec3& contact, float damage, bool crit);

} Meta (Enable, DisplayName( "DamageOnCollide" ));
