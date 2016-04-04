/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BossPhase3WaitTillTriggerState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossPhase3WaitTillTriggerState.h"

#include "BossAIStateMachine.h"
#include "BossAIComponent.h"
#include "Phase3LightTriggerComponent.h"

#include <World.h>

using namespace ursine;
using namespace ecs;

BossPhase3WaitTillTriggerState::BossPhase3WaitTillTriggerState(void)
    : BossAIState( "Wait Till Trigger Phase 3" )
    , m_finished( false ) { }

void BossPhase3WaitTillTriggerState::Enter(BossAIStateMachine *machine)
{
    auto world = machine->GetBoss( )->GetOwner( )->GetWorld( );

    world->Listener( this )
        .On( BOTH_SIDES_TRIGGERED, &BossPhase3WaitTillTriggerState::onTrigger );
}

void BossPhase3WaitTillTriggerState::onTrigger(EVENT_HANDLER(World))
{
    EVENT_SENDER(World, sender);

    sender->Listener( this )
        .Off( BOTH_SIDES_TRIGGERED, &BossPhase3WaitTillTriggerState::onTrigger );

    m_finished = true;
}
