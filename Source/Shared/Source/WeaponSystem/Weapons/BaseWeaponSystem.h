/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BaseWeaponSystem.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#pragma once

#include <FilterSystem.h>

#include <Randomizer.h>

#include "AbstractWeapon.h"

namespace ursine
{
    namespace ecs
    {
        class PhysicsSystem;
        class AudioEmitter;
        class Animator;
    }

    namespace physics
    {
        struct RaycastOutput;
    }
}

struct AbstractProjWeapon;
struct AbstractHitscanWeapon;
struct HitscanWeapon;
struct BaseWeapon;

class WeaponSystemUtils
{
public:
    static void GetSpreadValues(
        ursine::Randomizer& spread, 
        float accuracy, float& xSpread, float& ySpread
    );

    static void ConstructRaycast(
        AbstractWeapon& weapon, 
        ursine::SVec3& start, ursine::SVec3& end
    );

    static void ProjectileVelocity(
        AbstractProjWeapon& weapon, 
        ursine::ecs::EntityHandle& proj
    );

    static void ProjectileSetUp(
        ursine::ecs::EntityHandle& proj, 
        AbstractProjWeapon& weapon
    );

    static bool OutofAmmo(AbstractWeapon &weapon);

    static bool ClipFull(AbstractWeapon &weapon);

    static int Reload(AbstractWeapon &weapon);

    static void DecrementFireTimer(float dt, AbstractWeapon &weapon);

    static void DecrementReloadTimer(float dt, AbstractWeapon &weapon);

    static int RemoveRoundsFromClip(AbstractWeapon &weapon);

    static void ReloadWeapon(AbstractWeapon &weapon, ursine::ecs::AudioEmitter *emitter);

    static void ResetIdleSequence(AbstractWeapon* weapon);
};

class BaseWeaponSystem
    : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;

public:
    BaseWeaponSystem(ursine::ecs::World *world);

protected:
    void onUpdate(EVENT_HANDLER(World)) override;
    void Enable(const ursine::ecs::EntityHandle &entity) override;
    void Disable(const ursine::ecs::EntityHandle &entity) override;

private:
    void EvaluateProjectileWeapons(const float dt);

    void FireProjectileWeapon(AbstractProjWeapon &weapon, const ursine::ecs::EntityHandle &entity);

    void CreateProjectiles(AbstractProjWeapon& weapon, ursine::ecs::Transform& trans, const int projectilesFired);
    
    std::unordered_map<ursine::ecs::EntityHandle, AbstractProjWeapon*> m_weapons;
    std::unordered_map<ursine::ecs::EntityHandle, ursine::ecs::Transform*> m_transforms;
    std::unordered_map<ursine::ecs::EntityHandle, ursine::ecs::AudioEmitter*> m_emitters;

} Meta(Enable, AutoAddEntitySystem);

class HitscanWeaponSystem
    : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;

public:
    HitscanWeaponSystem(ursine::ecs::World *world);

protected:
    void Initialize(void) override;
    void onUpdate(EVENT_HANDLER(World)) override;
    void Enable(const ursine::ecs::EntityHandle &entity) override;
    void Disable(const ursine::ecs::EntityHandle &entity) override;

private:
    void EvaluateHitscanWeapons(const float dt);

    void FireHitscanWeapon(AbstractHitscanWeapon &weapon, const ursine::ecs::EntityHandle &handle);

    void CreateRaycasts(AbstractHitscanWeapon& weapon, ursine::ecs::Transform& trans, const int projectilesFired);
    bool RaycastClosestHitLogic(ursine::SVec3& raycastVec, ursine::physics::RaycastOutput& rayout, AbstractHitscanWeapon& weapon);

    void GetSpawnLocation(const ursine::ecs::EntityHandle &other, ursine::physics::RaycastOutput& rayout, ursine::SVec3& posToSet);
    void SpawnCollisionParticle(ursine::SVec3& collisionPoint, ursine::SVec3& normalizeRaycastVec, const ursine::ecs::EntityHandle &other);

    void CreateTrail(AbstractHitscanWeapon &weapon, ursine::SVec3 &trailEnd);

    ursine::ecs::PhysicsSystem *m_physicsSystem;

    std::unordered_map<ursine::ecs::EntityHandle, HitscanWeapon*> m_weapons;
    std::unordered_map<ursine::ecs::EntityHandle, ursine::ecs::Transform*> m_transforms;
    std::unordered_map<ursine::ecs::EntityHandle, ursine::ecs::AudioEmitter*> m_emitters;
} Meta(Enable, AutoAddEntitySystem);
