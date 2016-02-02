/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BaseWeaponComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/


#pragma once

#include "AbstractWeapon.h"
#include <Component.h>

struct BaseWeapon : ursine::ecs::Component, AbstractWeapon
{
    NATIVE_COMPONENT

public:

    AbstractWeaponFields( )

    BaseWeapon(void);
    ~BaseWeapon(void);

    void OnInitialize(void) override;

} Meta(Enable, DisplayName("ProjectileWeapon"));

