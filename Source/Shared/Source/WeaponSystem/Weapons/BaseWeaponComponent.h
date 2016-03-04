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

#include "AbstractProjWeapon.h"
#include <Component.h>

struct BaseWeapon : ursine::ecs::Component, AbstractProjWeapon
{
    NATIVE_COMPONENT

public:

    PROJ_EDITOR_FIELDS( );

    BaseWeapon(void);
    ~BaseWeapon(void);

    void OnInitialize(void) override;

protected:
    void RemoveMySelf(void) override;

} Meta(Enable , DisplayName("ProjectileWeapon"));
