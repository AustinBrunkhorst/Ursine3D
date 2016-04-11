/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VineGoHomeState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "VineGoHomeState.h"

#include "VineStateUtils.h"
#include "VineAIStateMachine.h"
#include "VineAIComponent.h"

#include <Application.h>
#include <ParticleEmitterComponent.h>
#include <Model3DComponent.h>
#include <EntityEvent.h>

using namespace ursine;
using namespace ecs;

VineGoHomeState::VineGoHomeState(void)
    : VineAIState( "Go Home" )
    , m_finished( false )
    , m_animating( false )
    , m_state( GoHomeState::Burrowing )
{
}

void VineGoHomeState::Enter(VineAIStateMachine *machine)
{
    m_finished = false;
    m_animating = false;
    m_state = GoHomeState::Burrowing;
}

void VineGoHomeState::Update(VineAIStateMachine *machine)
{
    if (m_animating || m_finished)
        return;

    auto ai = machine->GetAI( );
    auto aiOwner = ai->GetOwner( );
    auto animator = ai->GetAnimator( );

    switch (m_state)
    {
        case GoHomeState::Burrowing:
        {
            // Play the animation
            playAnimation( animator, "Spike_Down" );
            break;
        }
        case GoHomeState::Digging:
        {
            auto aiTrans = aiOwner->GetTransform( );
            auto aiPos = aiTrans->GetWorldPosition( );
            auto dt = Application::Instance->GetDeltaTime( );

            // move towards the target in the x-z plane.
            auto lookAtPosition = ai->GetHomeLocation( );

            if (machine->GetBool( VineAIStateMachine::JumpToHome ))
            {
                aiTrans->SetWorldPosition( lookAtPosition );
                m_state = GoHomeState::Uprooting;
                return;
            }

            lookAtPosition.Y( ) = aiPos.Y( );

            // Turn towards the target
            aiTrans->LookAt( lookAtPosition );

            // raycast to find what the Y position should be
            aiPos.Y( ) = VineStateUtils::FindYPosition( ai, aiPos );

            // Move forward based on the dig speed
            aiTrans->SetWorldPosition( aiPos + aiTrans->GetForward( ) * ai->GetDigSpeed( ) * dt );

            // Tell the particle emitter to play
            auto entity = aiOwner->GetChildByName( ai->GetDigParticleEmitterName( ) );

            auto emitters = entity->GetComponentsInChildren<ParticleEmitter>( );

            for (auto &emitter : emitters)
                emitter->SetEmitting( true );

            // Check to see if we've reached a valid distance
            if (VineStateUtils::AtHome( ai, 1.0f ))
            {
                m_state = GoHomeState::Uprooting;
            }

            break;
        }
        case GoHomeState::Uprooting:
        {
            playAnimation( animator, "Spike_Up" );

            auto models = aiOwner->GetComponentsInChildren<Model3D>( );

            for (auto &model : models)
                model->SetActive( true );

            // We've successfully pursued our target
            machine->SetBool( VineAIStateMachine::GoHome, false );
            machine->SetBool( VineAIStateMachine::IsHome, true );

            break;
        }
    }
}

void VineGoHomeState::playAnimation(Animator *animator, const std::string &clip)
{
    animator->SetCurrentState( clip );

    m_animating = true;

    animator->GetOwner( )->Listener( this )
        .On( ENTITY_ANIMATION_FINISH, &VineGoHomeState::onAnimationFinished );
}

void VineGoHomeState::onAnimationFinished(EVENT_HANDLER(Entity))
{
    EVENT_ATTRS(Entity, EventArgs);

    switch (m_state)
    {
        case GoHomeState::Burrowing:
        {
            // tell all models to turn themselves off
            auto models = sender->GetComponentsInChildren<Model3D>( );

            for (auto &model : models)
                model->SetActive( false );

            m_state = GoHomeState::Digging;
            break;
        }
        case GoHomeState::Uprooting:
        {
            m_finished = true;

            auto ai = sender->GetComponentInParents<VineAI>( );

            auto entity = ai->GetOwner( )->GetChildByName( ai->GetDigParticleEmitterName( ) );

            auto emitters = entity->GetComponentsInChildren<ParticleEmitter>( );

            for (auto &emitter : emitters)
                emitter->SetEmitting( false );

            break;
        }
    }

    m_animating = false;

    sender->Listener( this )
        .Off( ENTITY_ANIMATION_FINISH, &VineGoHomeState::onAnimationFinished );
}
