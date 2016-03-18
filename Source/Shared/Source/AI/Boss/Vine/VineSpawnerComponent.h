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

#include "LevelSegments.h"

class VineSpawner : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    VineSpawner(void);

    EditorField(
        LevelSegments spawnSegment,
        GetSpawnSegment,
        SetSpawnSegment
    );

    LevelSegments GetSpawnSegment(void) const;
    void SetSpawnSegment(LevelSegments segment);

private:

    LevelSegments m_spawnSegment;

} Meta(Enable);
