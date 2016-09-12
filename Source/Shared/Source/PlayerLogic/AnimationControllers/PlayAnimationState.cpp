/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayAnimationState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "PlayAnimationState.h"

#include "AnimatorControllerStateMachine.h"

#include <AnimatorComponent.h>
#include <EntityEvent.h>

using namespace ursine;
using namespace ecs;

PlayAnimationState::PlayAnimationState(
    const std::string &stateName,
    bool blocking, float playback, bool disableBoneManip
)
    : AnimatorControllerState( "Play Animation State: " + stateName )
    , m_stateName( stateName )
    , m_blocking( blocking )
    , m_playback( playback )
    , m_finished( !blocking )
    , m_disableBoneManip( disableBoneManip ) { }

void PlayAnimationState::Enter(AnimatorControllerStateMachine *machine)
{
    auto animator = machine->GetAnimator( );

    animator->SetCurrentState( m_stateName );
    animator->SetPlaying( true );
    animator->SetTimeScalar( m_playback );

    if (m_disableBoneManip)
        animator->SetEnableBoneManipulation( false );

    if (m_blocking)
    {
        animator->GetOwner( )->Listener( this )
            .On( ENTITY_ANIMATION_FINISH, &PlayAnimationState::onAnimationFinish );

        m_finished = false;
    }
}

void PlayAnimationState::Exit(AnimatorControllerStateMachine *machine)
{
    auto animator = machine->GetAnimator( );

    animator->SetTimeScalar( 1.0f );

    if (m_disableBoneManip)
        animator->SetEnableBoneManipulation( true );
}

void PlayAnimationState::onAnimationFinish(EVENT_HANDLER(Entity))
{
    EVENT_SENDER(Entity, sender);

    m_finished = true;

    sender->Listener( this )
        .Off( ENTITY_ANIMATION_FINISH, &PlayAnimationState::onAnimationFinish );
}
