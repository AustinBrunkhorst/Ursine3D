#include "Precompiled.h"

#include "CharacterControllerSystem.h"
#include "CharacterControllerComponent.h"
#include "CharacterFireControllerComponent.h"

#include "RigidbodyComponent.h"

#include "PlayerInputComponent.h"

#include <GamepadManager.h>
#include <MouseManager.h>
#include <KeyboardManager.h>
#include <UrsineMath.h>
#include <Core/Physics/Interop/Raycasting.h>
#include <PhysicsSystem.h>
#include <SystemManager.h>
#include <Components/HealthComponent.h>

using namespace ursine;
using namespace ursine::ecs;

ENTITY_SYSTEM_DEFINITION( CharacterControllerSystem );

CharacterControllerSystem::CharacterControllerSystem(ursine::ecs::World *world)
    : FilterSystem(world, Filter( ).All<CharacterController, Rigidbody, PlayerInput>( ))
{
    
}

void CharacterControllerSystem::Process(Entity *entity)
{
    auto *controller = entity->GetComponent<CharacterController>( );
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

    if (input->Jump( ))
        rigidbody->AddForce({ 0.0f, controller->jumpSpeed, 0.0f });
}
