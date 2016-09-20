/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Phase3BossSpawnComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class Phase3BossSpawn : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    Phase3BossSpawn(void);

} Meta(Enable);
