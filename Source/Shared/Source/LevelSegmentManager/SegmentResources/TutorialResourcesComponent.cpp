/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TutorialResourcesComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "TutorialResourcesComponent.h"

NATIVE_COMPONENT_DEFINITION( TutorialResources );

TutorialResources::TutorialResources(void)
    : BaseComponent( )
{
}

const ursine::resources::ResourceReference &TutorialResources::GetWorldData(void) const
{
    return m_worldToMerge;
}

void TutorialResources::SetWorldData(const ursine::resources::ResourceReference &world)
{
    m_worldToMerge = world;

    if (!resourcesAreAvailable( ))
        return;

    NOTIFY_COMPONENT_CHANGED( "world", m_worldToMerge );
}
