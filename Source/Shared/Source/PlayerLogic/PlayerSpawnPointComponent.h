/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerSpawnPointComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include "LevelSegments.h"

class PlayerSpawnPoint : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    PlayerSpawnPoint(void);

    EditorField(
        std::string playerArchetype,
        GetPlayerArchetype,
        SetPlayerArchetype
    );

    EditorField(
        LevelSegments spawnSegment,
        GetSpawnSegment,
        SetSpawnSegment
    );

    const std::string &GetPlayerArchetype(void) const;
    void SetPlayerArchetype(const std::string &archetype);

    LevelSegments GetSpawnSegment(void);
    void SetSpawnSegment(LevelSegments segment);

private:
    std::string m_archetype;

    LevelSegments m_segment;

} Meta(Enable);
