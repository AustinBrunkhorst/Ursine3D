#include "Precompiled.h"

#include "CharacterFireControllerSystem.h"
#include "CharacterFireControllerComponent.h"

#include "RigidbodyComponent.h"

#include "PlayerInputComponent.h"
#include "AudioEmitterComponent.h"

#include <GamepadManager.h>
#include <MouseManager.h>
#include <KeyboardManager.h>
#include <UrsineMath.h>
#include <Core/Physics/Interop/Raycasting.h>
#include <PhysicsSystem.h>
#include <SystemManager.h>
#include <Components/HealthComponent.h>
#include <AnimatorComponent.h>

using namespace ursine;
using namespace ursine::ecs;

namespace
{
	const std::string FireGun = "FIRE_GUN_HAND";
	const std::string kTakeDamage = "PLAYER_TAKE_DAMAGE";
	const std::string kEndRound = "ROUND_END_RED";
}

ENTITY_SYSTEM_DEFINITION( CharacterFireControllerSystem );

CharacterFireControllerSystem::CharacterFireControllerSystem( ursine::ecs::World *world )
    : FilterSystem( world, Filter( ).All<CharacterFireController>( ) )
{

}

void CharacterFireControllerSystem::Process( Entity *entity )
{
    auto *input = entity->GetTransform( )->GetRoot( )->GetOwner( )->GetComponent<PlayerInput>( );
    auto *fireController = entity->GetComponent<CharacterFireController>( );
    auto *entityTransform = entity->GetTransform( );
	auto *emitter = entity->GetTransform( )->GetRoot( )->GetOwner( )->GetComponent<AudioEmitterComponent>( );

    // check our states
    if ( input && input->ResetTrigger( ) )
    {
        fireController->SetFireState( true );
    }

    // update fire timer
    fireController->DecrementFireTimer( 0.016 );

    // find the child hotspot for firing
    auto childrenVector = entity->GetChildren( );

    Entity *hotspot = nullptr;
	Entity *arm = nullptr;

    for ( auto &x : *childrenVector )
    {
        auto *currentChild = m_world->GetEntity( x );

        if ( currentChild->GetName( ) == "FiringHotspot" )
        {
            hotspot = currentChild;
            continue;
        }
		if (currentChild->GetName( ) == "FPSArm")
		{
			arm = currentChild;
			continue;
		}
    }

    // firing a ray
    if ( hotspot != nullptr && input->Fire( ) && fireController->CanFire() )
    {
        // animation stuff
        auto *armAnimator = arm->GetComponent<Animator>( );

        // reset firing sequence
        armAnimator->SetAnimationTimePosition( 0.1 );
        armAnimator->SetTimeScalar( 1.0f / fireController->GetFireRate( ) );

        // Play that bang sound
		if (emitter)
			emitter->AddSoundToPlayQueue(FireGun);
            

        fireController->Fire( );

        //// get the camera, then get the arm connected to the camera
        //auto children = entity->GetChildren( );
        //auto camera = m_world->GetEntity( (*children)[ 0 ] );
        //auto camChildren = camera->GetChildren( );
        //auto armGun = m_world->GetEntity( (*camChildren)[ 0 ] );

        m_rng.SetMin( -fireController->GetAccuracy( ) );
        m_rng.SetMax( fireController->GetAccuracy( ) );

        float randomPitch = m_rng.GetValue( );
        float randomYaw = m_rng.GetValue( );

        SVec3 spray = entityTransform->GetUp( ) * randomPitch + entityTransform->GetRight( ) * randomYaw;
        SVec3 offset = entityTransform->GetWorldRotation( ) * fireController->GetFireOffset( );

        auto *childTransform = hotspot->GetTransform( );

        //auto *gunTransform = armGun->GetTransform( );
        physics::RaycastInput rayInput = physics::RaycastInput(
            childTransform->GetWorldPosition( ) + offset,
            childTransform->GetWorldPosition( ) + entityTransform->GetForward( ) * fireController->GetFireRange() + spray
        );

        physics::RaycastOutput rayOutput;

        m_world->GetEntitySystem( PhysicsSystem )->Raycast( 
            rayInput, 
            rayOutput, 
            physics::RAYCAST_ALL_HITS, 
            true, 
            0.1, 
            true, 
            fireController->GetShotStartColor(),
            fireController->GetShotEndColor( )
        );

        for ( auto &x : rayOutput.entity )
        {
            auto *hitObj = m_world->GetEntityUnique( x );
            auto *health = hitObj->GetComponent<Health>( );
            if ( health != nullptr )
            {
                health->DealDamage( fireController->GetDamage() );
				if (emitter)
					emitter->AddSoundToPlayQueue(kTakeDamage);
            }
        }
    }
}
