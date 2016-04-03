/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ToggleLightGroupState.cpp
**
** Author:
** - JordanEllis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ToggleLightGroupState.h"

#include "SegmentLogicStateMachine.h"
#include "LevelSegmentManagerComponent.h"
#include "LightingGroupComponent.h"

ToggleLightGroupState::ToggleLightGroupState(bool toggle, std::vector<std::string> groupEntities)
    : SegmentLogicState( "Toggle Light Groups" )
    , m_toggle( toggle )
    , m_groupEntities( groupEntities ) { }

void ToggleLightGroupState::Enter(SegmentLogicStateMachine *machine)
{
    auto world = machine->GetSegmentManager( )->GetOwner( )->GetWorld( );

    for (auto &groupName : m_groupEntities)
    {
        auto group = world->GetEntityFromName( groupName );

        UAssert( group, "Error: The entity name " + groupName + " does not exist in the world." );

        auto lightingGroup = group->GetComponent<LightingGroup>( );

        UAssert( lightingGroup, "Error: The entity specified is missing it's lighting group component." );

        lightingGroup->ToggleGroupLights( m_toggle );
    }
}
