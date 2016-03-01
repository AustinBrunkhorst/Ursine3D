/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VineWhipState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "VineWhipState.h"

#include "BossVineStateMachine.h"
#include "BossVineAIComponent.h"

#include <Application.h>

using namespace ursine;

VineWhipState::VineWhipState(void)
    : BossVineState( "Whip State" )
    , m_finished( false )
    , m_animating( false )
    , m_state( WhipState::In )
    , m_swingTimer( 0.0f )
{
}

void VineWhipState::Enter(BossVineStateMachine *machine)
{
    m_finished = false;
    m_animating = false;
    m_state = WhipState::In;
}

void VineWhipState::Update(BossVineStateMachine *machine)
{
    static const float goToDuration = 0.5f;

    auto ai = machine->GetAI( );
    auto aiTrans = ai->GetOwner( )->GetTransform( );
    auto animator = ai->GetAnimator( );

    switch (m_state)
    {
        case WhipState::In:
        {
            // tell the animator to whip in
            if (!m_animating)
            {
                playAnimation( animator, "WhipIn" );

                auto aiTrans = ai->GetOwner( )->GetTransform( );

                // Save the forward direction
                m_middleOrientation = aiTrans->GetWorldRotation( );

                float angle = ai->GetWhipAngle( ) * 0.5f;

                // Save the start orientation vector
                m_beginOrientation = SQuat( 0.0f, angle, 0.0f ) * m_middleOrientation;

                // Save the end orientation vector
                m_endOrientation = SQuat( 0.0f, -angle, 0.0f ) * m_middleOrientation;

                // randomize the direction
                if (rand( ) %2)
                    std::swap( m_beginOrientation, m_endOrientation );

                m_swingTimer = 0.0f;
            }
            else
            {
                m_swingTimer += Application::Instance->GetDeltaTime( );

                float t = m_swingTimer / goToDuration;

                if (t >= 1.0f)
                    t = 1.0f;

                aiTrans->SetWorldRotation( m_middleOrientation.Slerp( m_beginOrientation, t ) );
            }

            break;
        }
        case WhipState::Rotate:
        {
            m_swingTimer += Application::Instance->GetDeltaTime( );
            float t = m_swingTimer / ai->GetWhipDuration( );

            if (t >= 1.0f)
            {
                t = 1.0f;
                m_state = WhipState::Out;
            }

            aiTrans->SetWorldRotation( m_beginOrientation.Slerp( m_endOrientation, t ) );

            break;
        }
        case WhipState::Out:
        {
            // when that finishes, whip out
            if (!m_animating)
            {
                playAnimation( animator, "WhipOut" );
                m_swingTimer = 0.0f;
            }
            else
            {
                m_swingTimer += Application::Instance->GetDeltaTime( );

                float t = m_swingTimer / goToDuration;

                if (t >= 1.0f)
                    t = 1.0f;

                aiTrans->SetWorldRotation( m_endOrientation.Slerp( m_middleOrientation, t ) );
            }
            break;
        }
    }
}

void VineWhipState::Exit(BossVineStateMachine *machine)
{
    machine->SetFloat( "Cooldown", machine->GetAI( )->GetWhipCooldown( ) );
}

void VineWhipState::playAnimation(EntityAnimator *animator, const std::string &clip)
{
    animator->Play( clip );

    m_animating = true;

    animator->Listener( this )
        .On( EntityAnimatorEvent::FinishedAnimating, &VineWhipState::onAnimationFinished );
}

void VineWhipState::onAnimationFinished(EVENT_HANDLER(EntityAnimator))
{
    EVENT_ATTRS(EntityAnimator, ursine::ecs::EntityEventArgs);

    switch (m_state)
    {
        case WhipState::In:
        {
            m_state = WhipState::Rotate;
            break;
        }
        case WhipState::Out:
        {
            m_finished = true;
            break;
        }
    }

    m_animating = false;
    m_swingTimer = 0.0f;

    sender->Listener( this )
        .Off( EntityAnimatorEvent::FinishedAnimating, &VineWhipState::onAnimationFinished );
}
