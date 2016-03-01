/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VineUprootState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "VineUprootState.h"

#include "BossVineStateMachine.h"
#include "BossVineAIComponent.h"
#include "EntityAnimatorComponent.h"

using namespace ursine;

VineUprootState::VineUprootState(void)
    : BossVineState( "Uproot State" )
    , m_finished( false )
    , m_animating( false )
    , m_state( UprootState::Burrowing )
{
}

void VineUprootState::Enter(BossVineStateMachine *machine)
{
    m_finished = false;
    m_animating = false;
    m_state = UprootState::Burrowing;
}

void VineUprootState::Update(BossVineStateMachine *machine)
{
    if (m_animating)
        return;

    auto ai = machine->GetAI( );
    auto animator = ai->GetAnimator( );

    switch (m_state)
    {
        case UprootState::Burrowing:
        {
            playAnimation( animator, "Burrow" );
            break;
        }
        case UprootState::Digging:
        {
            // move towards the target in the x-z plane.
            // raycast to find what the Y position should be
            // when we first enter this state, tell a particle emitter to play
            // spawn spheres?
            break;
        }
        case UprootState::UprootDelay:
        {
            // When in this state, tell a particle emitter to amp up

            // decrement a timer
            // switch to uproot when timer is up
            break;
        }
        case UprootState::Uprooting:
        {
            playAnimation( animator, "Uproot" );
            break;
        }
    }
}

void VineUprootState::Exit(BossVineStateMachine *machine)
{
    
}

void VineUprootState::playAnimation(EntityAnimator *animator, const std::string &clip)
{
    animator->Play( clip );

    m_animating = true;

    animator->Listener( this )
        .On( EntityAnimatorEvent::FinishedAnimating, &VineUprootState::onAnimationFinished );
}

void VineUprootState::onAnimationFinished(EVENT_HANDLER(EntityAnimator))
{
    EVENT_ATTRS(EntityAnimator, ursine::ecs::EntityEventArgs);

    m_animating = false;

    sender->Listener( this )
        .Off( EntityAnimatorEvent::FinishedAnimating, &VineUprootState::onAnimationFinished );
}
