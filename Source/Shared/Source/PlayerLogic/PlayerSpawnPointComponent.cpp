/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerSpawnPointComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "PlayerSpawnPointComponent.h"

NATIVE_COMPONENT_DEFINITION( PlayerSpawnPoint );

using namespace ursine;
using namespace ecs;

PlayerSpawnPoint::PlayerSpawnPoint(void)
    : BaseComponent( )
    , m_segment( LevelSegments::Empty )
{
}

const resources::ResourceReference &PlayerSpawnPoint::GetPlayerArchetype(void) const
{
    return m_archetype;
}

void PlayerSpawnPoint::SetPlayerArchetype(const resources::ResourceReference &archetype)
{
    m_archetype = archetype;

    if (!resourcesAreAvailable( ))
        return;

    NOTIFY_COMPONENT_CHANGED( "playerArchetype", m_archetype );
}

LevelSegments PlayerSpawnPoint::GetSpawnSegment(void)
{
    return m_segment;
}

void PlayerSpawnPoint::SetSpawnSegment(LevelSegments segment)
{
    m_segment = segment;
}
