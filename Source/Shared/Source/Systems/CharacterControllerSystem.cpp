#include "Precompiled.h"

#include "CharacterControllerSystem.h"
#include "CharacterControllerComponent.h"
#include "CharacterFireControllerComponent.h"

#include "RigidbodyComponent.h"

#include "PlayerInputComponent.h"
#include "AudioEmitterComponent.h"

#include "SpawnSystem.h"

#include <EntitySystem.h>
#include <GamepadManager.h>
#include <MouseManager.h>
#include <KeyboardManager.h>
#include <UrsineMath.h>
#include <Core/Physics/Interop/Raycasting.h>
#include <PhysicsSystem.h>
#include <SystemManager.h>
#include <AudioSystem.h>
#include <Components/HealthComponent.h>

using namespace ursine;
using namespace ursine::ecs;

namespace
{
	const std::string kJumpSound = "PLAYER_JUMP";
	const std::string kLandSound = "PLAYER_LAND";
	const std::string kRunSound = "PLAYER_STEP";
	const float Runduration = 0.2f;
	const float Jumpduration = 1.29f;
}

ENTITY_SYSTEM_DEFINITION( CharacterControllerSystem );

CharacterControllerSystem::CharacterControllerSystem(ursine::ecs::World *world)
    : FilterSystem(world, Filter( ).All<CharacterController, Rigidbody, PlayerInput>( ))
{
    
}

void CharacterControllerSystem::Process(Entity *entity)
{
    auto *controller = entity->GetComponent<CharacterController>( );
    auto *emitter = entity->GetComponent<AudioEmitterComponent>( );
    auto *input = entity->GetComponent<PlayerInput>( );
    auto moveSpeed = controller->moveSpeed;
	auto rotateSpeed = controller->rotateSpeed;

    auto transform = entity->GetTransform( );
    auto rigidbody = entity->GetComponent<Rigidbody>( );
    
    float x = input->LookDir( ).X( );

	auto child = transform->GetChild(0);

	// This is an immidiate fix, cause fuck eet. - Jordan
	rigidbody->SetGravity( SVec3( 0.0f, -100.0f, 0.0f ) );

    if (abs( x ) > 0.1f)
    {
		// Get the first child (model + camera) and rotate it.
        float angle = x * rotateSpeed;

		child->SetWorldRotation( child->GetWorldRotation( ) * SQuat( 0.0f, angle, 0.0f ) );
    }

    auto move = input->MoveDir( ) * moveSpeed;

    auto forward = child->GetForward( ) * move.Y( );
    auto strafe = child->GetRight( ) * move.X( );
    auto vel = rigidbody->GetVelocity( );
    auto accum = forward + strafe;

	if (emitter)
	{
		if (input->Jump() && controller->CanJump)
		{
			vel.Y() = controller->jumpSpeed;
			emitter->AddSoundToPlayQueue(kJumpSound);
			controller->CanJump = false;
			controller->inAir = true;
			m_timers.Create(TimeSpan::FromSeconds(Jumpduration)).Completed([=] {
				if (entity)
				{
					auto *jump = entity->GetComponent<CharacterController>();
					if (jump)
					{
						jump->CanJump = true;
					}
				}
			});
		}
		else if (move != Vec2::Zero() && controller->CanStep && controller->CanJump)
		{
			emitter->AddSoundToPlayQueue(kRunSound);
			controller->CanStep = false;
			m_timers.Create(TimeSpan::FromSeconds(Runduration)).Completed([=] {
				if (entity)
				{
					auto *step = entity->GetComponent<CharacterController>();
					if (step)
						step->CanStep = true;
				}
			});
		}
		if (controller->CanJump && controller->inAir)
		{
			emitter->AddSoundToPlayQueue(kLandSound);
			controller->inAir = false;
		}
	}

    rigidbody->SetVelocity({ accum.X( ), vel.Y( ), accum.Z( ) });
}
