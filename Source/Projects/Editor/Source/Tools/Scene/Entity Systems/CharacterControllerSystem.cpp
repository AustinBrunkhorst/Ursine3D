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
    auto speed = controller->speed;

    auto gamepadManager = GetCoreSystem( ursine::GamepadManager );

    auto state = gamepadManager->GetState( id );

    if (state)
    {
        auto rightStick = state->Sticks( ).Right( );
        auto angle = ursine::math::RadiansToDegrees( rightStick.Angle( ) );

        entity->GetTransform( )->SetWorldRotation(
            ursine::SQuat( 0.0f, angle, 0.0f )
        );
    }
}
