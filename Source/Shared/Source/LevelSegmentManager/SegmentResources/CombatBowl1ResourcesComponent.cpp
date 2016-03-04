/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CombatBowl1ResourcesComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "CombatBowl1ResourcesComponent.h"

NATIVE_COMPONENT_DEFINITION( CombatBowl1Resources );

CombatBowl1Resources::CombatBowl1Resources(void)
    : BaseComponent( )
{
}

const ursine::resources::ResourceReference &CombatBowl1Resources::GetWorldData(void) const
{
    return m_worldToMerge;
}

void CombatBowl1Resources::SetWorldData(const ursine::resources::ResourceReference &world)
{
    m_worldToMerge = world;
}
