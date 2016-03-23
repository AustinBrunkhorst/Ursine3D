/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PollinateProjectileComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include <GhostComponent.h>
#include "DamageOnCollideComponent.h"

class PollinateProjectile : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

private:

} Meta(
    Enable, 
    RequiresComponents(
        typeof(DamageOnCollide), 
        typeof(ursine::ecs::Ghost)
    )
);
