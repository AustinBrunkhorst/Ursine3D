/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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
#include "LevelSegmentManagerComponent.h"

class Spawner;

class SpawnerGroup : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

    friend class SpawnerGroupSystem;
    friend class SpawnPattern;

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
    ~SpawnerGroup(void);

    AIArchetype GetEnemyType(void) const;
    void SetEnemyType(AIArchetype enemyType);

    int GetActiveEnemiesCount(void) const;

private:

    AIArchetype m_enemyType;

    int m_activeEnemies;

    std::unordered_map<AIArchetype, Spawner *> m_spawners;

    // Connect to the level manager's events
    void OnInitialize(void) override;

    // Add or remove a spawner form my map
    void addSpawner(Spawner *spawner);
    void removeSpawner(Spawner *spawner);

    // Check to see if we have a spawner of the given type
    bool haveSpawnerOfType(AIArchetype type);

    // Update function (called via SpawnerGroupSystem)
    void update(void);

    void onLevelSegmentChange(EVENT_HANDLER(LevelSegmentManager));

} Meta(Enable);