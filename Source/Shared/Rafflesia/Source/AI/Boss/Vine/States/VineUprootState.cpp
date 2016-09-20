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

#include "VineStateUtils.h"
#include "VineAIStateMachine.h"
#include "VineAIComponent.h"
#include "EntityAnimatorComponent.h"

#include <Application.h>
#include <Entity.h>
#include <ParticleEmitterComponent.h>
#include <Model3DComponent.h>
#include <BoxColliderComponent.h>
#include <GhostComponent.h>
#include <EntityEvent.h>
#include <UrsineMath.h>

using namespace ursine;
using namespace ecs;

VineUprootState::VineUprootState(void)
    : VineAIState( "Uproot State" )
    , m_finished( false )
    , m_animating( false )
    , m_delayTimer( 0.0f )
    , m_goHomeTime( 10.0f )
    , m_goHomeTimer( 0.0f )
    , m_state( UprootState::Burrowing )
{
}

void VineUprootState::Enter(VineAIStateMachine *machine)
{
    m_finished = false;
    m_animating = false;
    m_state = UprootState::Burrowing;
    machine->SetBool( VineAIStateMachine::IsHome, false );

    auto health = machine->GetAI( )->GetOwner( )->GetComponent<Health>( );

    health->Listener( this )
        .On( HEALTH_ZERO, &VineUprootState::onVineDeath );
}

void VineUprootState::Update(VineAIStateMachine *machine)
{
    if (m_animating || m_finished)
        return;

    auto ai = machine->GetAI( );
    auto aiOwner = ai->GetOwner( );
    auto animator = ai->GetAnimator( );

    switch (m_state)
    {
        case UprootState::Burrowing:
        {
            playAnimation( animator, "Spike_Down" );
            aiOwner->GetComponent<AudioEmitter>( )->PushEvent( ai->GetBurrowSfx( ) );
            break;
        }
        case UprootState::Digging:
        {
            UAssert( ai->GetTarget( ), "Error: There aren't players to target." );

            auto aiTrans = aiOwner->GetTransform( );
            auto aiPos = aiTrans->GetWorldPosition( );
            auto dt = Application::Instance->GetDeltaTime( );

            // move towards the target in the x-z plane.
            auto targetTrans = ai->GetTarget( )->GetTransform( );
            auto lookAtPosition = targetTrans->GetWorldPosition( );

            // add some randomness to it
            lookAtPosition.X( ) += math::Rand( -10.0f, 10.0f );
            lookAtPosition.Z( ) += math::Rand( -10.0f, 10.0f );

            // raycast to find what the Y position should be
            aiPos.Y( ) = VineStateUtils::FindYPosition( ai, aiPos );

            lookAtPosition.Y( ) = aiPos.Y( );

            // Turn towards the target
            aiTrans->LookAt( lookAtPosition, ai->GetDigTurnSpeed( ) );

            // Move forward based on the dig speed
            aiTrans->SetWorldPosition( aiPos + aiTrans->GetForward( ) * ai->GetDigSpeed( ) * dt );

            // Tell the particle emitter to play
            auto entity = aiOwner->GetChildByName( ai->GetDigParticleEmitterName( ) );

            auto emitters = entity->GetComponentsInChildren<ParticleEmitter>( );

            for (auto &emitter : emitters)
                emitter->SetEmitting( true );

            // Check to see if we've reached a valid distance
            if (VineStateUtils::AtTarget( ai, ai->GetUprootDistance( ) ))
            {
                m_state = UprootState::UprootDelay;
                m_delayTimer = ai->GetUprootDelay( );

                // correct the look at position if it is too close to another vine
                correctPosition( ai->GetOwner( ), lookAtPosition );

                aiTrans->SetWorldPosition( lookAtPosition );

                return;
            }

            // incrmenet the timer
            m_goHomeTimer += dt;

            // Tell it to go home if it's taking too long
            if (m_goHomeTimer >= m_goHomeTime)
            {
                m_state = UprootState::GoingHome;
            }

            break;
        }
        case UprootState::GoingHome:
        {
            auto aiTrans = aiOwner->GetTransform( );
            auto aiPos = aiTrans->GetWorldPosition( );
            auto dt = Application::Instance->GetDeltaTime( );

            // move towards the target in the x-z plane.
            auto lookAtPosition = ai->GetHomeLocation( );

            // raycast to find what the Y position should be
            aiPos.Y( ) = VineStateUtils::FindYPosition( ai, aiPos );

            lookAtPosition.Y( ) = aiPos.Y( );

            // Turn towards the target
            aiTrans->LookAt( lookAtPosition );

            // Move forward based on the dig speed
            aiTrans->SetWorldPosition( aiPos + aiTrans->GetForward( ) * ai->GetDigSpeed( ) * dt );

            // Tell the particle emitter to play
            auto entity = aiOwner->GetChildByName( ai->GetDigParticleEmitterName( ) );

            auto emitters = entity->GetComponentsInChildren<ParticleEmitter>( );

            for (auto &emitter : emitters)
                emitter->SetEmitting( true );

            // Check to see if we've reached a valid distance
            if (VineStateUtils::AtHome( ai, 5.0f ))
            {
                m_state = UprootState::UprootDelay;

                machine->SetBool( VineAIStateMachine::GoHome, false );
                machine->SetBool( VineAIStateMachine::IsHome, true );
                machine->SetBool( VineAIStateMachine::PursueTarget, false );

                aiTrans->SetWorldPosition( ai->GetHomeLocation( ) );
            }
        }
        case UprootState::UprootDelay:
        {
            // decrement a timer
            m_delayTimer -= Application::Instance->GetDeltaTime( );

            // switch to uproot when timer is up
            if (m_delayTimer <= 0.0f)
            {
                m_state = UprootState::Uprooting;
            }

            break;
        }
        case UprootState::Uprooting:
        {
            playAnimation( animator, "Spike_Up" );

            auto models = aiOwner->GetComponentsInChildren<ecs::Model3D>( );

            for (auto &model : models)
                model->SetActive( true );

            // Update the vine's "time of last pursue"
            ai->m_timeOfLastPursue = Application::Instance->GetTimeSinceStartup( );

            // turn off the emitters
            auto entity = aiOwner->GetChildByName( ai->GetDigParticleEmitterName( ) );

            auto emitters = entity->GetComponentsInChildren<ParticleEmitter>( );

            for (auto &emitter : emitters)
                emitter->SetEmitting( false );

            aiOwner->GetComponent<AudioEmitter>( )->PushEvent( ai->GetEmergeSfx( ) );

            break;
        }
    }
}

void VineUprootState::Exit(VineAIStateMachine *machine)
{
    machine->SetFloat(
        VineAIStateMachine::UprootCooldown,
        machine->GetAI( )->GetUprootCooldown( )
    );

    auto health = machine->GetAI( )->GetOwner( )->GetComponent<Health>( );

    health->Listener( this )
        .Off( HEALTH_ZERO, &VineUprootState::onVineDeath );

    auto animator = machine->GetAI( )->GetAnimator( );
    
    if (animator)
        animator->GetOwner( )->Listener( this )
            .Off( ENTITY_ANIMATION_FINISH, &VineUprootState::onAnimationFinished );
}

void VineUprootState::playAnimation(Animator *animator, const std::string &clip)
{
    animator->SetCurrentState( clip );

    m_animating = true;

    animator->GetOwner( )->Listener( this )
        .On( ENTITY_ANIMATION_FINISH, &VineUprootState::onAnimationFinished );
}

void VineUprootState::onAnimationFinished(EVENT_HANDLER(Entity))
{
    EVENT_ATTRS(Entity, EntityEventArgs);

    switch (m_state)
    {
        case UprootState::Burrowing:
        {
            // tell all models to turn themselves off
            auto models = sender->GetComponentsInChildren<Model3D>( );

            for (auto &model : models)
                model->SetActive( false );

            m_state = UprootState::Digging;

            m_goHomeTimer = 0.0f;

            break;
        }
        case UprootState::Uprooting:
        {
            m_finished = true;

            break;
        }
    }

    m_animating = false;

    sender->Listener( this )
        .Off( ENTITY_ANIMATION_FINISH, &VineUprootState::onAnimationFinished );
}

void VineUprootState::onVineDeath(EVENT_HANDLER(Health))
{
    m_finished = true;
}

void VineUprootState::correctPosition(EntityHandle thisVine, SVec3 &position)
{
    // iterate through all vines and see if position is too close
    auto vines = thisVine->GetWorld( )->GetEntitiesFromFilter(
        Filter( ).All<VineAI>( )
    );

    auto thisPos = position;

    thisPos.Y( ) = 0.0f;

    for (auto &vine : vines)
    {
        if (vine != thisVine)
        {
            auto pos = vine->GetTransform( )->GetWorldPosition( );

            pos.Y( ) = 0.0f;

            auto dist = SVec3::Distance( pos, thisPos );

            if (dist < 10.0f)
            {
                auto dir = SVec3::Normalize( -pos );

                position += dir * dist;
                return;
            }
        }
    }
}
