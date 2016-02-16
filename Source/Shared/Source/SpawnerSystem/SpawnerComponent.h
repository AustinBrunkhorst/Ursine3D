/* ----------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SpawnerComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - CJ Payne
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class Spawner : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    // Button for adding a pattern (for a level + sub level event)
    EditorButton(
        createPattern,
        "Create New Pattern"
    );

    Spawner(void);

} Meta(Enable, HiddenInSelector, DisableComponentRemoval);
