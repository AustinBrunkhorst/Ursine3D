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
#include <BoxColliderComponent.h>

class Health;

class DamageOnCollide : public ursine::ecs::Component
{
    NATIVE_COMPONENT;
public:

    DamageOnCollide( void );
    ~DamageOnCollide( void );

    void OnInitialize( void ) override;

    float GetDamageToApply( void ) const;
    void SetDamageToApply( const float damage );

    float GetCritModifier( void ) const;
    void SetCritModifier( const float modifier );

    float GetDamageInterval( void ) const;
    void SetDamageInterval( const float damageInterval );

    const std::string& GetArchetypeOnDeath(void) const;
    void SetArchetypeOnDeath(const std::string& objToSpawn);

    const std::string& GetArchetypeOnHit(void) const;
    void SetArchetypeOnHit(const std::string& objToSpawn);

    bool GetDeleteOnCollision( void ) const;
    void SetDeleteOnCollision( const bool state );

    bool GetSpawnOnDeath(void) const;
    void SetSpawnOnDeath(const bool state);

    bool GetSpawnOnHit(void) const;
    void SetSpawnOnHit(const bool state);

    void OnCollide(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED));

    void ApplyDamage(Health* healthComp);

    // add an entity to the damage intervals so it can not be damaged by this object during 
    //   damage interval
    void AddEntityToIntervals(ursine::ecs::EntityUniqueID uniqueID);

    void DecrementDamageIntervalTimes( const float dt );

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

    void OnDeath(EVENT_HANDLER(ursine::ecs::ENTITY_REMOVED));

    Meta(Disable)
    bool DeleteOnCollision(void);

    // map of all objects hit
    std::unordered_map<ursine::ecs::EntityUniqueID, float> m_damageTimeMap;

} Meta (Enable, DisplayName( "DamageOnCollide" ), RequiresComponents(typeof(ursine::ecs::BoxCollider)));
