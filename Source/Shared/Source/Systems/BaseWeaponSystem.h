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

} // ursine namespace

struct AbstractWeapon;
struct AbstractHitscanWeapon;
struct BaseWeapon;
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

    void FireProjectileWeapon(AbstractWeapon& weapon, ursine::ecs::EntityUniqueID id);

    void CreateProjectiles(AbstractWeapon& weapon, ursine::ecs::Transform& trans, const int projectilesFired);
    
    std::unordered_map < ursine::ecs::EntityUniqueID, BaseWeapon* > m_weapons;
    std::unordered_map < ursine::ecs::EntityUniqueID, ursine::ecs::Transform* > m_transforms;
    std::unordered_map < ursine::ecs::EntityUniqueID, ursine::ecs::AudioEmitter* > m_emitters;

} Meta(Enable);


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

    ursine::ecs::PhysicsSystem* m_physicsSystem;

    std::unordered_map < ursine::ecs::EntityUniqueID, HitscanWeapon* > m_weapons;
    std::unordered_map < ursine::ecs::EntityUniqueID, ursine::ecs::Transform* > m_transforms;
    std::unordered_map < ursine::ecs::EntityUniqueID, ursine::ecs::AudioEmitter* > m_emitters;


} Meta(Enable);



