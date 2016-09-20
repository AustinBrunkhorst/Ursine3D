/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SlimePitComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class SlimePit : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    SlimePit(void);

} Meta( Enable );
