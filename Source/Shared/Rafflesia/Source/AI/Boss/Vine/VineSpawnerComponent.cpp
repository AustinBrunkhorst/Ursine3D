/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** VineSpawnerComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "VineSpawnerComponent.h"

NATIVE_COMPONENT_DEFINITION( VineSpawner );

VineSpawner::VineSpawner(void)
    : BaseComponent( )
    , m_spawnSegment( LevelSegments::Empty )
{
}

LevelSegments VineSpawner::GetSpawnSegment(void) const
{
    return m_spawnSegment;
}

void VineSpawner::SetSpawnSegment(LevelSegments segment)
{
    m_spawnSegment = segment;
}
