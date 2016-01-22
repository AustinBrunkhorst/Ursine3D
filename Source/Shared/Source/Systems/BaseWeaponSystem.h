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

#include <FilterSystem.h>

namespace ursine
{
    class KeyboardManager;
}

class BaseWeapon;

class BaseWeaponSystem
    : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;

public:
    BaseWeaponSystem(ursine::ecs::World* world);

protected:
    void OnInitialize(void) override;
    void Begin(void) override;
    void Process(ursine::ecs::Entity* entity) override;

private:
    float m_dt;

    void EvaluateWeapon(BaseWeapon& weapon);
    void ShootWeapon( BaseWeapon& weapon );
    void ReloadWeapon( BaseWeapon& weapon ) const;
    void CreateProjectiles(BaseWeapon& weapon, int projectilesFired );

} Meta(Enable);



