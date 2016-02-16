/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SpawnPatterContainerComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - CJ Payne
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include "SpawnPattern.h"

#include "LevelEvents.h"

class SpawnPatternContainer : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    SpawnPatternContainer(void);

    // The level event these spawn patterns are for
    LevelEvents levelEvent;

    // Array of spawn patterns
    ursine::Array<SpawnPattern> spawnPatterns;

} Meta(Enable, HiddenInSelector, DisableComponentRemoval);
