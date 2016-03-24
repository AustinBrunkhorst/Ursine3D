/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VineDeathState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "VineDeathState.h"

#include "VineAIStateMachine.h"
#include "VineAIComponent.h"
#include <EntityEvent.h>

using namespace ursine;
using namespace ecs;

VineDeathState::VineDeathState(void)
    : VineAIState( "Death" )
{
}

void VineDeathState::Enter(VineAIStateMachine *machine)
{
    auto ai = machine->GetAI( );
    auto animator = ai->GetAnimator( );

    owner = ai->GetOwner( );

    animator->SetCurrentState( "Spike_Down" );

    animator->GetOwner( )->Listener( this )
        .On( ENTITY_ANIMATION_FINISH, &VineDeathState::onAnimationFinished );
}

void VineDeathState::onAnimationFinished(EVENT_HANDLER(Entity))
{
    owner->Delete( );
}
