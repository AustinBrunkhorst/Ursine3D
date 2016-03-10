/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BossRoomResourcesComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossRoomResourcesComponent.h"

NATIVE_COMPONENT_DEFINITION( BossRoomResources );

BossRoomResources::BossRoomResources(void)
    : BaseComponent( )
{
}

const ursine::resources::ResourceReference &BossRoomResources::GetWorldData(void) const
{
    return m_worldToMerge;
}

void BossRoomResources::SetWorldData(const ursine::resources::ResourceReference &world)
{
    m_worldToMerge = world;

    if (!resourcesAreAvailable( ))
        return;

    NOTIFY_COMPONENT_CHANGED( "world", m_worldToMerge );
}
