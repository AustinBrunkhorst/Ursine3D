/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CharacterFireControllerSystem.cpp
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "CharacterFireControllerSystem.h"
#include "CharacterFireControllerComponent.h"

#include "AudioEmitterComponent.h"

#include <Raycasting.h>
#include <PhysicsSystem.h>
#include <SystemManager.h>
#include <Components/HealthComponent.h>
#include <AnimatorComponent.h>

using namespace ursine;
using namespace ecs;

namespace
{
	const std::string kFireGun = "FIRE_GUN_HAND";
	const std::string kTakeDamage = "PLAYER_TAKE_DAMAGE";
}

ENTITY_SYSTEM_DEFINITION( CharacterFireControllerSystem );

CharacterFireControllerSystem::CharacterFireControllerSystem(World *world)
    : FilterSystem( world, Filter( ).All<CharacterFireController>( ) )
{

}

void CharacterFireControllerSystem::Process(Entity *entity)
{
    auto *fireController = entity->GetComponent<CharacterFireController>( );
    auto *entityTransform = entity->GetTransform( );
    auto *emitter = entity->GetTransform( )->GetRoot( )->GetOwner( )->GetComponent<AudioEmitter>( );

    if (fireController == nullptr)
    {
        return;
    }

    // update fire timer
    fireController->DecrementFireTimer( Application::Instance->GetDeltaTime( ) );

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

	auto *armAnimator = arm->GetComponent<Animator>();
	armAnimator->UpdateAnimation(0.016f);

    // firing a ray
    // if we had a hotspot, firing controller is ready to fire (firing timer is down), AND we received input from
    // command (IsFiring), fire
    if ( hotspot != nullptr &&  fireController->CanFire() )
    {
        // animation stuff
        
        float animationScalar = armAnimator->GetTimeScalar( );

        // reset firing sequence
        armAnimator->SetAnimationTimePosition( 0.1f );
        armAnimator->SetTimeScalar(1.2f);
        armAnimator->SetAnimation( "Gun_Shoot" );
        armAnimator->SetPlaying( true );

        // Play that bang sound
		if (emitter)
			emitter->AddSoundToPlayQueue(kFireGun);
            

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
            0.1f, 
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
                health->DealDamage( fireController->GetDamage( ) );
				if (emitter)
					emitter->AddSoundToPlayQueue( kTakeDamage );
            }
        }
    }
    else if (fireController->GetFireTimer( ) < 0.0f)
    {
        // reset idle sequence
        armAnimator->SetTimeScalar( 1.0f );
        armAnimator->SetAnimation( "Gun_Idle" );
        armAnimator->SetPlaying( true );
    }
}
