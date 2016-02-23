/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SpawnPatter.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - CJ Payne
** --------------------------------------------------------------------------*/

#pragma once

#include "LevelSegments.h"

class SpawnerGroup;
class Spawner;
class SpawnPatternContainer;

class SpawnPattern
{
public:

    Meta(MultiLineEditor)
    std::string notes;

    EditorField(
        bool active,
        GetActive,
        SetActive
    );

    EditorField(
        int totalEnemies,
        GetTotalEnemies,
        SetTotalEnemies
    );

    EditorField(
        float spawnDuration,
        GetSpawnDuration,
        SetSpawnDuration
    );

    EditorField(
        int maxNumberOfEnemies,
        GetMaxNumberOfEnemies,
        SetMaxNumberOfEnemies        
    );

    EditorField(
        int minNumberOfEnemies,
        GetMinNumberOfEnemies,
        SetMinNumberOfEnemies
    );

    EditorField(
        float singleSpawnCooldown,
        GetSingleSpawnCooldown,
        SetSingleSpawnCooldown
    );

    EditorField(
        float spawnerBreakTime,
        GetSpawnerBreakTime,
        SetSpawnerBreakTime
    );

    EditorField(
        bool triggerNextPattern,
        GetTriggerNextPattern,
        SetTriggerNextPattern
    );

    EditorField(
        bool loopPattern,
        GetLoopPattern,
        SetLoopPattern
    );

   EditorField(
        LevelSegments endingLevelSegmentTransition,
        GetEndingSegmentTransition,
        SetEndingSegmentTransition
    );

    EditorField(
        float spawnDistanceVariance,
        GetSpawnDistanceVariance,
        SetSpawnDistanceVariance
    );

    EditorField(
        ursine::SVec3 direction,
        GetSpawnDirection,
        SetSpawnDirection
    );

    Meta(InputRange(0.0f, 180.0f, 0.1f))
    EditorField(
        float spawnAngleVariance,
        GetSpawnAngleVariance,
        SetSpawnAngleVariance
    );

    SpawnPattern(void);

    bool GetActive(void) const;
    void SetActive(bool active);

    int GetTotalEnemies(void) const;
    void SetTotalEnemies(int total);

    float GetSpawnDuration(void) const;
    void SetSpawnDuration(float duration);

    int GetMaxNumberOfEnemies(void) const;
    void SetMaxNumberOfEnemies(int max);

    int GetMinNumberOfEnemies(void) const;
    void SetMinNumberOfEnemies(int min);

    float GetSingleSpawnCooldown(void) const;
    void SetSingleSpawnCooldown(float cooldown);

    float GetSpawnerBreakTime(void) const;
    void SetSpawnerBreakTime(float breakTime);

    bool GetTriggerNextPattern(void) const;
    void SetTriggerNextPattern(bool flag);

    bool GetLoopPattern(void) const;
    void SetLoopPattern(bool loop);

    LevelSegments GetEndingSegmentTransition(void) const;
    void SetEndingSegmentTransition(LevelSegments segment);

    float GetSpawnDistanceVariance(void) const;
    void SetSpawnDistanceVariance(float variance);

    const ursine::SVec3 &GetSpawnDirection(void) const;
    void SetSpawnDirection(const ursine::SVec3 &direction);

    float GetSpawnAngleVariance(void) const;
    void SetSpawnAngleVariance(float variance);

    // Update this pattern
    void Update(SpawnerGroup *group, Spawner *spawner, SpawnPatternContainer *container);

private:

    // When checked, the pattern will be used by it's spawner
    bool m_active : 1; // CHECK

    // Whether this pattern ending automatically causes the next 
    // pattern for the same sub-level event to begin
    bool m_triggerNextPattern : 1; // CHECK

    // When checked, this pattern will restart when it ends
    bool m_loopPattern : 1; // CHECK

    // The segment to transition to once the pattern finishes
    LevelSegments m_endingSegmentTransition; // CHECK

    // The number of enemies to be spawned in this pattern
    int m_totalEnemies; // CHECK

    // The maximum number of enemies which can be active at once
    int m_maxNumberOfEnemies; // CHECK

    // The minimum number of enemies which can be active at once
    int m_minNumberOfEnemies; // CHECK

    // The cooldown time after spawning a single enemy
    float m_singleSpawnCooldown; // CHECK

    // The duration for which this pattern lasts
    float m_spawnDuration; // CHECK

    // The amount of time the spawner pauses for after the pattern ends
    float m_spawnerBreakTime;

    // The distance from the spawner this pattern spawns
    float m_spawnDistanceVariance; // CHECK

    // The variance in degrees which the direction can be rancomized
    // from it's set value (above)
    float m_spawnAngleVariance; // CHECK

    // The direction this pattern will spawn in
    ursine::SVec3 m_spawnDirection; // CHECK

    ////////////////////////////////////////////////////
    // Private section for update function
    ////////////////////////////////////////////////////

    // Number of alive and active enemies this pattern has spawned
    int m_activeEnemies;

    // Number of total enemies spawned by this pattern
    int m_totalEnemiesSpawned;

    // Timer letting us know how long it has been since we last spawned an enemy
    float m_spawnTimer;

    // Timer for breaking (after one loop)
    float m_breakTimer;

    // Flag letting us know if we're currently breaking
    bool m_breaking;

    SpawnerGroup *m_group;

    Spawner *m_spawner;

    void spawn(SpawnerGroup *group, Spawner *spawner, SpawnPatternContainer *container);

    void onEnemyDeath(EVENT_HANDLER(Entity));

} Meta(Enable, EnableArrayType);
