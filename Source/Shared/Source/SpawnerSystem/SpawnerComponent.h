/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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
#include <ArchetypeData.h>

#include "AIArchetypes.h"
#include "LevelSegments.h"

class SpawnPatternContainer;
class SpawnerGroup;

class Spawner : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

    friend class SpawnerGroup;
    friend class SpawnPattern;

public:

    // Button for adding a pattern (for a level + sub level event)
    EditorButton(
        createPattern,
        "Create New Pattern"
    );

    EditorResourceField(
        ursine::resources::ArchetypeData,
        enemyArchetype,
        GetEnemyArchetype,
        SetEnemyArchetype
    );

    Spawner(void);

    const ursine::resources::ResourceReference &GetEnemyArchetype(void) const;
    void SetEnemyArchetype(const ursine::resources::ResourceReference &archetype);

    AIArchetype GetEnemyType(void) const;

    int GetActiveEnemiesCount(void) const;

    void OnSerialize(ursine::Json::object &output) const override;
    void OnDeserialize(const ursine::Json &input) override;

private:

    ursine::resources::ResourceReference m_archetype;

    AIArchetype m_enemyType;

    int m_activeEnemies;

    // This vector contains all active patterns
    std::vector<SpawnPatternContainer*> m_activePatterns;

    void update(SpawnerGroup *group);

    // This is called by the spawner group when the level manager changes
    // the level segment
    void onLevelSegmentChange(LevelSegments segment);

    ursine::ecs::EntityHandle spawnEnemy(SpawnerGroup *group, const ursine::SVec3 &worldPosition);

} Meta(Enable, HiddenInSelector, DisableComponentRemoval);
