/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BaseWeaponSystem.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#pragma once

#include <EntitySystem.h>

namespace ursine
{
    namespace ecs
    {
        class PhysicsSystem;

    } // ecs namespace

} // ursine namespace

struct AbstractWeapon;
struct AbstractHitscanWeapon;

class BaseWeaponSystem
    : public ursine::ecs::EntitySystem
{
    ENTITY_SYSTEM;

public:
    BaseWeaponSystem(ursine::ecs::World* world);

protected:
    void OnInitialize(void) override;
    void OnRemove(void) override;
    void Process(EVENT_HANDLER(World));

private:

    // entity's components changed
    void OnComponentAdded(EVENT_HANDLER(World));

    // entity removed
    void OnComponentRemoved(EVENT_HANDLER(World));

    void EvaluateProjectileWeapons(const float dt);

    void FireProjectileWeapon(AbstractWeapon& weapon, ursine::ecs::Transform& trans);

    void CreateProjectiles(const AbstractWeapon& weapon, ursine::ecs::Transform& trans, const int projectilesFired);

    ursine::ecs::PhysicsSystem& m_physicsSystem;
    
    std::unordered_map < ursine::ecs::EntityUniqueID, AbstractWeapon* > m_weapons;
    std::unordered_map < ursine::ecs::EntityUniqueID, ursine::ecs::Transform* > m_transforms;

} Meta(Enable);


class HitscanWeaponSystem
    : public ursine::ecs::EntitySystem
{
    ENTITY_SYSTEM;

public:
    HitscanWeaponSystem(ursine::ecs::World* world);

protected:
    void OnInitialize(void) override;
    void OnRemove(void) override;
    void Process(EVENT_HANDLER(World));

private:

    // entity's components changed
    void OnComponentAdded(EVENT_HANDLER(World));

    // entity removed
    void OnComponentRemoved(EVENT_HANDLER(World));

    void EvaluateHitscanWeapons(const float dt);

    void FireHitscanWeapon(AbstractHitscanWeapon& weapon, ursine::ecs::Transform& trans);

    void CreateRaycasts(const AbstractHitscanWeapon& weapon, ursine::ecs::Transform& trans, const int projectilesFired);

    ursine::ecs::PhysicsSystem& m_physicsSystem;

    std::unordered_map < ursine::ecs::EntityUniqueID, AbstractHitscanWeapon* > m_weapons;
    std::unordered_map < ursine::ecs::EntityUniqueID, ursine::ecs::Transform* > m_transforms;

} Meta(Enable);



