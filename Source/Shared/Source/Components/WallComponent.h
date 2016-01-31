#pragma once
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


struct WallComponent : ursine::ecs::Component
{
    NATIVE_COMPONENT
public:

    WallComponent(void);
    ~WallComponent(void);

    void OnInitialize(void) override;

} Meta(Enable, DisplayName("WallComponent"));
