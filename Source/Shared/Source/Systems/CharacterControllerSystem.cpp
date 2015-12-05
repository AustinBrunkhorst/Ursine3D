#include "Precompiled.h"

#include "CharacterControllerSystem.h"
#include "CharacterControllerComponent.h"
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
	bool step = true;
	bool jump = false;
	bool land = false;
	float startHeight = 0.0f;
}

ENTITY_SYSTEM_DEFINITION( CharacterControllerSystem );

CharacterControllerSystem::CharacterControllerSystem(ursine::ecs::World *world)
    : FilterSystem(world, Filter( ).All<CharacterController, Rigidbody, PlayerInput>( ))
{
    
}

void CharacterControllerSystem::Process(Entity *entity)
{
    auto *controller = entity->GetComponent<CharacterController>( );
	auto *emitter = entity->GetComponent<AudioEmitterComponent>();
    auto *input = entity->GetComponent<PlayerInput>( );
    auto moveSpeed = controller->moveSpeed;
	auto rotateSpeed = controller->rotateSpeed;

    auto transform = entity->GetTransform( );
    auto rigidbody = entity->GetComponent<Rigidbody>( );
    
    float x = input->LookDir( ).X( );

    if (abs( x ) > 0.1f)
    {
        float angle = x * rotateSpeed;

        rigidbody->AddTorque({ 0.0f, angle, 0.0f });
    }
    else
        rigidbody->SetAngularVelocity({ 0.0f, 0.0f, 0.0f });

    auto move = input->MoveDir( ) * moveSpeed;

    auto forward = transform->GetForward( ) * move.Y( );
    auto strafe = transform->GetRight( ) * move.X( );
    auto vel = rigidbody->GetVelocity( );
    auto accum = forward + strafe;

    rigidbody->SetVelocity({ accum.X( ), vel.Y( ), accum.Z( ) });

    if (input->Jump( ) && !jump)
    {
		emitter->AddSoundToPlayQueue(kJumpSound);
		startHeight = transform->GetWorldPosition().Y();
		rigidbody->AddForce({ 0.0f, controller->jumpSpeed, 0.0f });
		jump = true;
		m_timers.Create(TimeSpan::FromSeconds(Jumpduration)).Completed([&] {
			jump = false;
			land = true;
		});
    }
	else if (move != Vec2::Zero() && step && !jump)
	{
		emitter->AddSoundToPlayQueue(kRunSound);
		step = false;
		m_timers.Create(TimeSpan::FromSeconds(Runduration)).Completed([&]{
			step = true;
		});
	}
	if (land)
	{
		emitter->AddSoundToPlayQueue(kLandSound);
		land = false;
	}
}
