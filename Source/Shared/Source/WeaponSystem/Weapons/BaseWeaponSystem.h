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

namespace ursine
{
    namespace ecs
    {
        class PhysicsSystem;
        class AudioEmitter;
        class Animator;
    } // ecs namespace

    namespace physics
    {
        struct RaycastOutput;
    } // physics namespace

} // ursine namespace

struct AbstractProjWeapon;
struct AbstractHitscanWeapon;
struct HitscanWeapon;

class BaseWeaponSystem
    : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;

public:
    BaseWeaponSystem(ursine::ecs::World* world);

protected:
    void onUpdate(EVENT_HANDLER(World)) override;
    void Enable(ursine::ecs::Entity *entity) override;
    void Disable(ursine::ecs::Entity *entity) override;
private:

    void EvaluateProjectileWeapons(const float dt);

    void FireProjectileWeapon(AbstractProjWeapon& weapon, ursine::ecs::EntityUniqueID id);

    void CreateProjectiles(AbstractProjWeapon& weapon, ursine::ecs::Transform& trans, const int projectilesFired);
    
    std::unordered_map < ursine::ecs::EntityUniqueID, AbstractProjWeapon* > m_weapons;
    std::unordered_map < ursine::ecs::EntityUniqueID, ursine::ecs::Transform* > m_transforms;
    std::unordered_map < ursine::ecs::EntityUniqueID, ursine::ecs::AudioEmitter* > m_emitters;

} Meta(Enable, AutoAddEntitySystem);


class HitscanWeaponSystem
    : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;

public:
    HitscanWeaponSystem(ursine::ecs::World* world);

protected:
    void Initialize(void) override;
    void onUpdate(EVENT_HANDLER(World)) override;
    void Enable(ursine::ecs::Entity *entity) override;
    void Disable(ursine::ecs::Entity *entity) override;

private:

    void EvaluateHitscanWeapons(const float dt);

    void FireHitscanWeapon(AbstractHitscanWeapon& weapon, ursine::ecs::EntityUniqueID id);

    void CreateRaycasts(AbstractHitscanWeapon& weapon, ursine::ecs::Transform& trans, const int projectilesFired);
    void RaycastClosestHitLogic(ursine::SVec3& raycastVec, ursine::physics::RaycastOutput& rayout, AbstractHitscanWeapon& weapon);

    void GetSpawnLocation(ursine::ecs::Entity* other, ursine::physics::RaycastOutput& rayout, ursine::SVec3& posToSet);
    void SpawnCollisionParticle(ursine::SVec3& collisionPoint, ursine::SVec3& normalizeRaycastVec, ursine::ecs::Entity* other);

    void CreateTrail(AbstractHitscanWeapon& weapon, ursine::SVec3& trailEnd);

    ursine::ecs::PhysicsSystem* m_physicsSystem;

    std::unordered_map < ursine::ecs::EntityUniqueID, HitscanWeapon* > m_weapons;
    std::unordered_map < ursine::ecs::EntityUniqueID, ursine::ecs::Transform* > m_transforms;
    std::unordered_map < ursine::ecs::EntityUniqueID, ursine::ecs::AudioEmitter* > m_emitters;


} Meta(Enable, AutoAddEntitySystem);
