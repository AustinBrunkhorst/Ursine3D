/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EndingCreditsResourcesComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "EndingCreditsResourcesComponent.h"

NATIVE_COMPONENT_DEFINITION( EndingCreditsResources );

EndingCreditsResources::EndingCreditsResources(void)
    : BaseComponent( ) { }

const ursine::resources::ResourceReference &EndingCreditsResources::GetWorldData(void) const
{
    return m_worldToMerge;
}

void EndingCreditsResources::SetWorldData(const ursine::resources::ResourceReference &world)
{
    m_worldToMerge = world;

    if (!resourcesAreAvailable( ))
        return;

    NOTIFY_COMPONENT_CHANGED( "world", m_worldToMerge );
}
