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

#include "VineAIStateMachine.h"
#include "VineAIComponent.h"

#include <Application.h>
#include <EntityEvent.h>

using namespace ursine;
using namespace ecs;

VineWhipState::VineWhipState(void)
    : VineAIState( "Whip State" )
    , m_finished( false )
    , m_animating( false )
    , m_state( WhipState::In )
    , m_swingTimer( 0.0f )
{
}

void VineWhipState::Enter(VineAIStateMachine *machine)
{
    m_finished = false;
    m_animating = false;
    m_state = WhipState::In;

    auto health = machine->GetAI( )->GetOwner( )->GetComponent<Health>( );

    health->Listener( this )
        .On( HEALTH_ZERO, &VineWhipState::onVineDeath );
}

void VineWhipState::Update(VineAIStateMachine *machine)
{
    static const float goToDuration = 0.5f;

    if (m_finished)
        return;

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
                playAnimation( animator, "Swipe_In" );

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
                playAnimation( animator, "Whip_Out" );
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

void VineWhipState::Exit(VineAIStateMachine *machine)
{
    machine->SetFloat( 
        VineAIStateMachine::WhipCooldown,
        machine->GetAI( )->GetWhipCooldown( )
    );

    auto health = machine->GetAI( )->GetOwner( )->GetComponent<Health>( );

    health->Listener( this )
        .Off( HEALTH_ZERO, &VineWhipState::onVineDeath );
}

void VineWhipState::playAnimation(Animator *animator, const std::string &clip)
{
    animator->SetCurrentState( clip );

    m_animating = true;

    animator->GetOwner( )->Listener( this )
        .On( ENTITY_ANIMATION_FINISH, &VineWhipState::onAnimationFinished );
}

void VineWhipState::onAnimationFinished(EVENT_HANDLER(Entity))
{
    EVENT_ATTRS(Entity, ursine::ecs::EntityEventArgs);

    switch (m_state)
    {
        case WhipState::In:
        {
            m_state = WhipState::Rotate;

            sender->GetComponent<Animator>( )->SetCurrentState( "Whip_Idle" );

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
        .Off( ENTITY_ANIMATION_FINISH, &VineWhipState::onAnimationFinished );
}

void VineWhipState::onVineDeath(EVENT_HANDLER(Health))
{
    m_finished = true;
}
