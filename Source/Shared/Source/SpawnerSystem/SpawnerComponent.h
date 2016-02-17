/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

#include "AIArchetypes.h"

class Spawner : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

    friend class SpawnerGroup;

public:

    // Button for adding a pattern (for a level + sub level event)
    EditorButton(
        createPattern,
        "Create New Pattern"
    );

    Spawner(void);

    AIArchetype GetEnemyType(void) const;

private:

    AIArchetype m_enemyType;

} Meta(Enable, HiddenInSelector, DisableComponentRemoval);
