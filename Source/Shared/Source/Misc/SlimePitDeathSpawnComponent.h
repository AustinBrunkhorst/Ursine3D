/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SlimePitDeathSpawnComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class SlimePitDeathSpawn : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    SlimePitDeathSpawn(void);

} Meta( Enable );
