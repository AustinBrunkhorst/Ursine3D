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
#include "EntityAnimatorComponent.h"

VineDeathState::VineDeathState(void)
    : VineAIState( "Death" )
{
}

void VineDeathState::Enter(VineAIStateMachine *machine)
{
    auto ai = machine->GetAI( );
    auto animator = ai->GetAnimator( );

    owner = ai->GetOwner( );

    animator->Play( "Burrow" );

    animator->Listener( this )
        .On( EntityAnimatorEvent::FinishedAnimating, &VineDeathState::onAnimationFinished );
}

void VineDeathState::onAnimationFinished(EVENT_HANDLER(EntityAnimator))
{
    owner->Delete( );
}
