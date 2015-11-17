#include "Precompiled.h"

#include "CharacterControllerSystem.h"
#include "CharacterControllerComponent.h"

#include "RigidbodyComponent.h"

#include <GamepadManager.h>
#include <UrsineMath.h>

using namespace ursine;
using namespace ursine::ecs;

ENTITY_SYSTEM_DEFINITION( CharacterControllerSystem );

CharacterControllerSystem::CharacterControllerSystem(ursine::ecs::World *world)
    : FilterSystem(world, Filter( ).All<CharacterController, Rigidbody>( ))
{
    
}

void CharacterControllerSystem::Process(Entity *entity)
{
    auto *controller = entity->GetComponent<CharacterController>( );
    auto id = controller->id;
    auto moveSpeed = controller->moveSpeed;
	auto rotateSpeed = controller->rotateSpeed;

    auto gamepadManager = GetCoreSystem( ursine::GamepadManager );

    auto state = gamepadManager->GetState( id );

    if (state)
    {
		auto dt = ursine::Application::Instance->GetDeltaTime( );
		auto transform = entity->GetTransform();
        auto rigidbody = entity->GetComponent<Rigidbody>();

		float x = abs(state->Sticks( ).Right( ).X( ));

		if (x > 0.1f)
		{
			auto angle = state->Sticks( ).Right( ).X( ) * rotateSpeed;

            rigidbody->AddTorque( SVec3( 0.0f, angle, 0.0f ) );
		}
        else
            rigidbody->SetAngularVelocity( SVec3( 0.0f, 0.0f, 0.0f ) );

		auto walk = state->Sticks( ).Left( ) * moveSpeed;

		auto forward = transform->GetForward( ) * walk.Y( );
		auto strafe = transform->GetRight( ) * walk.X( );
        auto vel = rigidbody->GetVelocity( );
        auto accum = forward + strafe;

        rigidbody->SetVelocity( { accum.X( ), vel.Y( ), accum.Z( ) } );

        if (state->IsButtonTriggeredDown( BTN_A ))
            rigidbody->AddForce( SVec3( 0.0f, controller->jumpSpeed, 0.0f ) );
    }
}
