/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** HitscanProjectile.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include "GfxAPI.h"

enum RaycastType;

struct HitscanProjectile : ursine::ecs::Component
{
    NATIVE_COMPONENT
public:

    HitscanProjectile(void);
    ~HitscanProjectile(void);

    void OnInitialize(void) override;

} Meta(Enable, DisplayName("HitscanProjectile"));


