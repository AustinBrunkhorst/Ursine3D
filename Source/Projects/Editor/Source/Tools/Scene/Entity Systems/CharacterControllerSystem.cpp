#include "Precompiled.h"

#include "CharacterControllerSystem.h"
#include "CharacterControllerComponent.h"

#include <GamepadManager.h>
#include <UrsineMath.h>

ENTITY_SYSTEM_DEFINITION( CharacterControllerSystem );

CharacterControllerSystem::CharacterControllerSystem(ursine::ecs::World *world)
    : FilterSystem(world, ursine::ecs::Filter( ).All<CharacterController>( ))
{
    
}

void CharacterControllerSystem::Process(ursine::ecs::Entity *entity)
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

		float x = abs(state->Sticks( ).Right( ).X( ));

		if (x > 0.1f)
		{
			auto angle = state->Sticks( ).Right( ).X( ) * rotateSpeed * dt;

			transform->SetWorldRotation(
				transform->GetWorldRotation( ) * ursine::SQuat( 0.0f, angle, 0.0f )
			);
		}

		auto walk = state->Sticks( ).Left( ) * moveSpeed * dt;

		auto forward = transform->GetForward( ) * walk.Y( );
		auto strafe = transform->GetRight( ) * walk.X( );

		transform->SetWorldPosition(
			transform->GetWorldPosition( ) + forward + strafe
		);
    }
}
