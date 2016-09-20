/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WallComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

struct Wall : ursine::ecs::Component
{
    NATIVE_COMPONENT
public:

    Wall(void);

} Meta(Enable);
