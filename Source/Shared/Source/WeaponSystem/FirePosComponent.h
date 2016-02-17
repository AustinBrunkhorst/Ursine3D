#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FirePos.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/


#include <Component.h>


struct FirePos : ursine::ecs::Component
{
    NATIVE_COMPONENT
public:

    FirePos(void);
    ~FirePos(void);
} Meta(Enable, DisplayName("FirePos"));
