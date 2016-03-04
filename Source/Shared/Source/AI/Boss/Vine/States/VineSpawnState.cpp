/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VineSpawnState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "VineSpawnState.h"

#include "VineAIStateMachine.h"
#include "VineAIComponent.h"
#include "EntityAnimatorComponent.h"
#include "EventArgs.h"

VineSpawnState::VineSpawnState(void)
    : VineAIState( "Spawn" )
    , m_finished( false )
{
}

void VineSpawnState::Enter(VineAIStateMachine *machine)
{
    auto animator = machine->GetAI( )->GetAnimator( );

    animator->Listener( this )
        .On( EntityAnimatorEvent::FinishedAnimating, &VineSpawnState::onAnimationFinished );

    animator->Play( "Uproot" );
}

void VineSpawnState::onAnimationFinished(EVENT_HANDLER(EntityAnimator))
{
    EVENT_ATTRS(EntityAnimator, ursine::EventArgs);

    m_finished = true;

    sender->Listener( this )
        .Off( EntityAnimatorEvent::FinishedAnimating, &VineSpawnState::onAnimationFinished );
}

