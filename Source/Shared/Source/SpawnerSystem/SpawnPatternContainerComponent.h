/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

#include "LevelSegments.h"

class SpawnerGroup;
class Spawner;

class SpawnPatternContainer : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

    friend class Spawner;

public:
    SpawnPatternContainer(void);

    // The level event these spawn patterns are for
    LevelSegments levelSegment;

    // Array of spawn patterns
    ursine::Array<SpawnPattern> spawnPatterns;

private:
    // An index into the spawn pattern array, letting us know
    // what pattern is the current one.
    int m_index;

    // The time counter letting us know how long this pattern
    // has been updating
    float m_patternTimer;

    // Flag letting us know if we're breaking right now or not
    bool m_breaking;

    void update(SpawnerGroup *group, Spawner *spawner);

} Meta(
    Enable
) EditorMeta(
    HiddenInSelector, 
    DisableComponentRemoval
);
 