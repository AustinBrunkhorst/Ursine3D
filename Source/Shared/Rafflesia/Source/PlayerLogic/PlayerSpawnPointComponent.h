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

#include <ArchetypeData.h>

class PlayerSpawnPoint : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    PlayerSpawnPoint(void);

    EditorResourceField(
        ursine::resources::ArchetypeData,
        playerArchetype,
        GetPlayerArchetype,
        SetPlayerArchetype
    );

    EditorField(
        LevelSegments spawnSegment,
        GetSpawnSegment,
        SetSpawnSegment
    );

    const ursine::resources::ResourceReference &GetPlayerArchetype(void) const;
    void SetPlayerArchetype(const ursine::resources::ResourceReference &archetype);

    LevelSegments GetSpawnSegment(void);
    void SetSpawnSegment(LevelSegments segment);

private:
    LevelSegments m_segment;

    ursine::resources::ResourceReference m_archetype;

} Meta(Enable);
