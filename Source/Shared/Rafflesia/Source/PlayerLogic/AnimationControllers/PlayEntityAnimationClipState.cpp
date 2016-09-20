/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayEntityAnimationClipState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "PlayEntityAnimationClipState.h"

#include "AnimatorControllerStateMachine.h"

#include <AnimatorComponent.h>

using namespace ursine;
using namespace ecs;

PlayEntityAnimationClipState::PlayEntityAnimationClipState(
    const std::string& clipName, bool blocking
)
    : AnimatorControllerState( "Play Entity Clip: " + clipName )
    , m_clipName( clipName )
    , m_blocking( blocking )
    , m_finished( !blocking ) { }

void PlayEntityAnimationClipState::Enter(AnimatorControllerStateMachine *machine)
{
    auto animator = machine->GetAnimator( );

    auto eAnimator = animator->GetOwner( )->GetComponent<EntityAnimator>( );

    UAssert( eAnimator, "You must have an entity animator to use this state." );

    eAnimator->Play( m_clipName );

    if (m_blocking)
    {
        eAnimator->Listener( this )
            .On( EntityAnimatorEvent::FinishedAnimating, &PlayEntityAnimationClipState::onAnimationFinished );
    }
}

void PlayEntityAnimationClipState::onAnimationFinished(EVENT_HANDLER(EntityAnimator))
{
    EVENT_SENDER(EntityAnimator, sender);

    m_finished = true;

    sender->Listener( this )
        .Off( EntityAnimatorEvent::FinishedAnimating, &PlayEntityAnimationClipState::onAnimationFinished );
}
