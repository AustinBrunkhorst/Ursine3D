/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** VineSpawnerComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class VineSpawner : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    VineSpawner(void);

} Meta(Enable);
