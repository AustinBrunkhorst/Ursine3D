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
    URSINE_TODO("Get acutal delta time for firing timer reduction");
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
    // if we had a hotspot, firing controller is ready to fire (firing timer is down), AND we received input from
    // command (IsFiring), fire
    if ( hotspot != nullptr &&  fireController->CanFire() && input->Fire( ) )
    {
        // animation stuff
        auto *armAnimator = arm->GetComponent<Animator>( );
        float animationScalar = armAnimator->GetTimeScalar( );

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

        // calculate offset for firing accuracy
        m_rng.SetMin( -fireController->GetAccuracy( ) );
        m_rng.SetMax( fireController->GetAccuracy( ) );

        float randomPitch = m_rng.GetValue( );
        float randomYaw = m_rng.GetValue( );

        // calculate spray, as well as the offset from firing point
        SVec3 spray = entityTransform->GetUp( ) * randomPitch + entityTransform->GetRight( ) * randomYaw;
        SVec3 offset = entityTransform->GetWorldRotation( ) * fireController->GetFireOffset( );

        auto *childTransform = hotspot->GetTransform( );

        // setting up the input to the raycast system
        physics::RaycastInput rayInput = physics::RaycastInput(
            childTransform->GetWorldPosition( ) + offset,
            childTransform->GetWorldPosition( ) + entityTransform->GetForward( ) * fireController->GetFireRange() + spray
        );

        physics::RaycastOutput rayOutput;

        // raycast
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

        // search through hit objects, looking for stuff we can damage
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
