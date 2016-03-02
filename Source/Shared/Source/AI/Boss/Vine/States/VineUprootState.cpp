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

#include "VineAIStateMachine.h"
#include "VineAIComponent.h"
#include "EntityAnimatorComponent.h"

#include <PhysicsSystem.h>
#include <Application.h>
#include <Entity.h>
#include <ParticleEmitterComponent.h>
#include <Model3DComponent.h>

using namespace ursine;

VineUprootState::VineUprootState(void)
    : VineAIState( "Uproot State" )
    , m_finished( false )
    , m_animating( false )
    , m_delayTimer( 0.0f )
    , m_state( UprootState::Burrowing )
{
}

void VineUprootState::Enter(VineAIStateMachine *machine)
{
    m_finished = false;
    m_animating = false;
    m_state = UprootState::Burrowing;
}

void VineUprootState::Update(VineAIStateMachine *machine)
{
    if (m_animating)
        return;

    auto ai = machine->GetAI( );
    auto aiOwner = ai->GetOwner( );
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
            UAssert( ai->GetTarget( ), "Error: There aren't players to target." );

            auto aiTrans = aiOwner->GetTransform( );
            auto aiPos = aiTrans->GetWorldPosition( );
            auto dt = Application::Instance->GetDeltaTime( );

            // move towards the target in the x-z plane.
            auto targetTrans = ai->GetTarget( )->GetTransform( );
            auto lookAtPosition = targetTrans->GetWorldPosition( );

            lookAtPosition.Y( ) = aiPos.Y( );

            // Turn towards the target
            aiTrans->LookAt( lookAtPosition, ai->GetDigTurnSpeed( ) );

            // raycast to find what the Y position should be
            aiPos.Y( ) = findYPosition( ai, aiPos );

            // Move forward based on the dig speed
            aiTrans->SetWorldPosition( aiPos + aiTrans->GetForward( ) * ai->GetDigSpeed( ) * dt );

            // Tell the particle emitter to play
            auto emitter = aiOwner->GetChildByName( ai->GetDigParticleEmitterName( ) );

            emitter->GetComponent<ecs::ParticleEmitter>( )->SetEmitRate( 200 );

            // Check to see if we've reached a valid distance
            if (closeToTarget( ai ))
            {
                m_state = UprootState::UprootDelay;
                m_delayTimer = ai->GetUprootDelay( );
            }

            break;
        }
        case UprootState::UprootDelay:
        {
            // When in this state, tell a particle emitter to amp up
            auto emitter = aiOwner->GetChildByName( ai->GetDigParticleEmitterName( ) );
            auto particleEmitter = emitter->GetComponent<ecs::ParticleEmitter>( );

            particleEmitter->SetVelocityRange( SVec3( 5.0f, 20.0f, 5.0f ) );

            // decrement a timer
            m_delayTimer -= Application::Instance->GetDeltaTime( );

            // switch to uproot when timer is up
            if (m_delayTimer <= 0.0f)
            {
                particleEmitter->SetEmitRate( 0 );
                m_state = UprootState::Uprooting;
            }

            break;
        }
        case UprootState::Uprooting:
        {
            playAnimation( animator, "Uproot" );

            auto models = aiOwner->GetComponentsInChildren<ecs::Model3D>( );

            for (auto &model : models)
                model->SetActive( true );

            break;
        }
    }
}

void VineUprootState::Exit(VineAIStateMachine *machine)
{
    
}

void VineUprootState::playAnimation(EntityAnimator *animator, const std::string &clip)
{
    animator->Play( clip );

    m_animating = true;

    animator->Listener( this )
        .On( EntityAnimatorEvent::FinishedAnimating, &VineUprootState::onAnimationFinished );
}

float VineUprootState::findYPosition(VineAI *ai, const SVec3 &aiPosition)
{
    auto aiOwner = ai->GetOwner( );
    auto world = aiOwner->GetWorld( );
    auto physics = world->GetEntitySystem<ecs::PhysicsSystem>( );

    auto startPos = aiPosition - SVec3::UnitY( ) * 50.0f;
    auto dir = SVec3::UnitY( ) * 1000.0f;

    physics::RaycastInput input( startPos, dir );
    physics::RaycastOutput output;

    if (!physics->Raycast( input, output, physics::RAYCAST_ALL_HITS, true ))
        return aiPosition.Y( );

    auto lowestY = std::numeric_limits<float>( ).max( );
    auto found = false;

    // iterate through all output and find the closest hit (that isn't the AI)
    for (int i = 0; i < output.entity.size( ); ++i)
    {
        auto hitEntity = world->GetEntityUnique( output.entity[ i ] );

        // make sure we're not checking for ourselves, and entities
        // we don't care about
        auto collisionItr = std::find(
            ai->collisionList.begin( ), 
            ai->collisionList.end( ),
            hitEntity->GetRoot( )->GetName( ) 
        );

        auto ignoreItr = std::find(
            ai->ignoreList.begin( ),
            ai->ignoreList.end( ),
            hitEntity->GetName( )
        );

        if (hitEntity == aiOwner->GetRoot( ) ||
            collisionItr == ai->collisionList.end( ) ||
            ignoreItr != ai->ignoreList.end( ))
            continue;

        auto hit = output.hit[ i ];

        if (hit.Y( ) < lowestY)
        {
            lowestY = hit.Y( );
            found = true;
        }
    }

    if (found)
        return lowestY;
    else
        return aiPosition.Y( );
}

bool VineUprootState::closeToTarget(VineAI *ai)
{
    auto targetTrans = ai->GetTarget( )->GetTransform( );
    auto aiTrans = ai->GetOwner( )->GetTransform( );

    float dist = SVec3::Distance( 
        targetTrans->GetWorldPosition( ), 
        aiTrans->GetWorldPosition( ) 
    );

    return dist <= ai->GetUprootDistance( );
}

void VineUprootState::onAnimationFinished(EVENT_HANDLER(EntityAnimator))
{
    EVENT_ATTRS(EntityAnimator, ursine::ecs::EntityEventArgs);

    switch (m_state)
    {
        case UprootState::Burrowing:
        {
            // tell all models to turn themselves off
            auto models = sender->GetOwner( )->GetComponentsInChildren<ecs::Model3D>( );

            for (auto &model : models)
                model->SetActive( false );

            m_state = UprootState::Digging;
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
        .Off( EntityAnimatorEvent::FinishedAnimating, &VineUprootState::onAnimationFinished );
}
