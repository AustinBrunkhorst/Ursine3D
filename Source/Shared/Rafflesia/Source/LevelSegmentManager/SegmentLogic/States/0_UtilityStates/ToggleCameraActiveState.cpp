/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ToggleCameraActiveState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ToggleCameraActiveState.h"

#include "SegmentLogicStateMachine.h"
#include "LevelSegmentManagerComponent.h"

#include <World.h>
#include <EntityHandle.h>
#include <CameraComponent.h>

using namespace ursine;
using namespace ecs;

ToggleCameraActiveState::ToggleCameraActiveState(const std::string &entityName, bool toggle)
    : SegmentLogicState( "Toggle Camera: " + entityName )
    , m_entityName( entityName )
    , m_toggle( toggle ) { }

void ToggleCameraActiveState::Enter(SegmentLogicStateMachine *machine)
{
    auto sm = machine->GetSegmentManager( );

    auto world = sm->GetOwner( )->GetWorld( );

    auto entity = world->GetEntityFromName( m_entityName );

    auto cam = entity->GetComponent<Camera>( );

    if (!cam)
        return;

    cam->SetActive( m_toggle );
}
