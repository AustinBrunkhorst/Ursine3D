/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayEntityAnimatorState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "PlayEntityAnimatorState.h"

#include "SegmentLogicStateMachine.h"
#include "LevelSegmentManagerComponent.h"

#include <World.h>

PlayEntityAnimatorState::PlayEntityAnimatorState(const std::string &entityName, bool blocking)
    : SegmentLogicState( "Play Entity Animator: " + entityName )
    , m_entityName( entityName )
    , m_blocking( blocking )
    , m_finished( !blocking ) { }

void PlayEntityAnimatorState::Enter(SegmentLogicStateMachine *machine)
{
    auto sm = machine->GetSegmentManager( );

    auto world = sm->GetOwner( )->GetWorld( );

    auto entity = world->GetEntityFromName( m_entityName );

    auto animator = entity->GetComponent<EntityAnimator>( );

    if (!animator)
        return;

    animator->Play( );

    if (m_blocking)
    {
        animator->Listener( this )
            .On( EntityAnimatorEvent::FinishedAnimating, &PlayEntityAnimatorState::onAnimationFinished );
    }
}

void PlayEntityAnimatorState::onAnimationFinished(EVENT_HANDLER(EntityAnimator))
{
    EVENT_SENDER(EntityAnimator, sender);

    sender->Listener( this )
        .Off( EntityAnimatorEvent::FinishedAnimating, &PlayEntityAnimatorState::onAnimationFinished );

    m_finished = true;
}
