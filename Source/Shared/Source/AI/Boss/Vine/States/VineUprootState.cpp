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

#include <PhysicsSystem.h>
#include <Application.h>
#include <Entity.h>

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
            aiPos.Y( ) = findYPosition( aiOwner->GetWorld( ), aiOwner, aiPos );

            // Move forward based on the dig speed
            aiTrans->SetWorldPosition( aiPos + aiTrans->GetForward( ) * ai->GetDigSpeed( ) * dt );

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

float VineUprootState::findYPosition(ecs::World* world, ecs::Entity *ai, const SVec3 &aiPosition)
{
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

        // TODO make this dynamic
        if (hitEntity == ai->GetRoot( ) || hitEntity->GetRoot( )->GetName( ) != "CombatBowl1Level")
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


void VineUprootState::onAnimationFinished(EVENT_HANDLER(EntityAnimator))
{
    EVENT_ATTRS(EntityAnimator, ursine::ecs::EntityEventArgs);

    switch (m_state)
    {
        case UprootState::Burrowing:
        {
            m_state = UprootState::Digging;
        }
    }

    m_animating = false;

    sender->Listener( this )
        .Off( EntityAnimatorEvent::FinishedAnimating, &VineUprootState::onAnimationFinished );
}
