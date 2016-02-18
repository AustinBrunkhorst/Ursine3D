/* ----------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SpawnerGroupComponent.h
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

class Spawner;

class SpawnerGroup : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

    friend class SpawnerGroupSystem;

public:

    EditorField(
        AIArchetype chooseEnemyType,
        GetEnemyType,
        SetEnemyType
    );

    // Button for adding a spawner for a given enemy type
    EditorButton(
        createSpawner,
        "Create New Spawner"
    );

    SpawnerGroup(void);

    AIArchetype GetEnemyType(void) const;
    void SetEnemyType(AIArchetype enemyType);

private:

    AIArchetype m_enemyType;

    std::unordered_map<AIArchetype, Spawner *> m_spawners;

    void addSpawner(Spawner *spawner);
    void removeSpawner(Spawner *spawner);

    bool haveSpawnerOfType(AIArchetype type);

} Meta(Enable);