/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Phase3WaitForTriggerState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "Phase3WaitForTriggerState.h"

#include "SegmentLogicStateMachine.h"
#include "LevelSegmentManagerComponent.h"
#include "Phase3LightTriggerComponent.h"

using namespace ursine;
using namespace ecs;

Phase3WaitForTriggerState::Phase3WaitForTriggerState(void)
    : SegmentLogicState( "Phase3 Wait For Trigger" )
    , m_finished( false ) { }

void Phase3WaitForTriggerState::Enter(SegmentLogicStateMachine *machine)
{
    auto sm = machine->GetSegmentManager( );
    auto world = sm->GetOwner( )->GetWorld( );

    world->Listener( this )
        .On( BOTH_SIDES_TRIGGERED, &Phase3WaitForTriggerState::onTrigger );

    m_finished = false;
}

void Phase3WaitForTriggerState::onTrigger(EVENT_HANDLER(World))
{
    EVENT_SENDER(World, sender);

    sender->Listener( this )
        .Off( BOTH_SIDES_TRIGGERED, &Phase3WaitForTriggerState::onTrigger );

    m_finished = true;
}
