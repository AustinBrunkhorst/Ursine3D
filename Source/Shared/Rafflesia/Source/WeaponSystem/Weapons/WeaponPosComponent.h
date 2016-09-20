/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WeaponPosComponent.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class WeaponPos : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    WeaponPos(void);
    
} Meta(Enable);
