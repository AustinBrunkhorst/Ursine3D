#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** HitsanWeaponComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include "AbstractHitscanWeapon.h"

struct HitscanWeapon : ursine::ecs::Component, AbstractHitscanWeapon
{
    NATIVE_COMPONENT
public:

    AbstractWeaponFields( );

    HitscanEditorFields( );

    HitscanWeapon(void);
    ~HitscanWeapon(void);

    void OnInitialize(void) override;

    void RemoveMySelf(void) override;

} Meta(Enable, DisplayName("HitscanWeapon"));
