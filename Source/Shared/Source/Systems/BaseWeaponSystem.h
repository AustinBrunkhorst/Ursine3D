/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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
    void Process(ursine::ecs::Entity* entity);

private:
    void EvaluateWeapon(BaseWeapon& weapon);
    void ShootWeapon( BaseWeapon& weapon );
    void ReloadWeapon( BaseWeapon& weapon );
    void CreateBullets(BaseWeapon& weapon, int bulletsFired );

} Meta(Enable);



