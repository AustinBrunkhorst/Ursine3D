#include "Precompiled.h"

#include "CharacterControllerSystem.h"
#include "CharacterControllerComponent.h"

#include "RigidbodyComponent.h"

#include <GamepadManager.h>
#include <MouseManager.h>
#include <KeyboardManager.h>
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

    auto dt = ursine::Application::Instance->GetDeltaTime();

    if (controller->keyboard)
    {
        auto transform = entity->GetTransform();
        auto rigidbody = entity->GetComponent<Rigidbody>();

        float x = GetCoreSystem( MouseManager )->GetPositionDelta( ).X( );

        if (abs(x) > 0.1f)
        {
            float angle = x * rotateSpeed;

            rigidbody->AddTorque(SVec3(0.0f, angle, 0.0f));
        }
        else
            rigidbody->SetAngularVelocity(SVec3(0.0f, 0.0f, 0.0f));

        auto keyboard = GetCoreSystem( KeyboardManager );
        Vec2 walk;

        if (keyboard->IsDown(KEY_A))
            walk.X( ) -= moveSpeed;
        if (keyboard->IsDown(KEY_D))
            walk.X( ) += moveSpeed;
        if (keyboard->IsDown(KEY_W))
            walk.Y( ) += moveSpeed;
        if (keyboard->IsDown(KEY_S))
            walk.Y( ) -= moveSpeed;

        auto forward = transform->GetForward() * walk.Y();
        auto strafe = transform->GetRight() * walk.X();
        auto vel = rigidbody->GetVelocity();
        auto accum = forward + strafe;

        rigidbody->SetVelocity({ accum.X(), vel.Y(), accum.Z() });

        if (keyboard->IsTriggeredDown(KEY_SPACE))
            rigidbody->AddForce(SVec3(0.0f, controller->jumpSpeed, 0.0f));
    }
    else
    {
        auto state = gamepadManager->GetState(id);

        if (state)
        {
            auto transform = entity->GetTransform();
            auto rigidbody = entity->GetComponent<Rigidbody>();

            float x = abs(state->Sticks().Right().X());

            if (x > 0.1f)
            {
                float angle = state->Sticks().Right().X() * rotateSpeed;

                rigidbody->AddTorque(SVec3(0.0f, angle, 0.0f));
            }
            else
                rigidbody->SetAngularVelocity(SVec3(0.0f, 0.0f, 0.0f));

            auto walk = state->Sticks().Left() * moveSpeed;

            auto forward = transform->GetForward() * walk.Y();
            auto strafe = transform->GetRight() * walk.X();
            auto vel = rigidbody->GetVelocity();
            auto accum = forward + strafe;

            rigidbody->SetVelocity({ accum.X(), vel.Y(), accum.Z() });

            if (state->IsButtonTriggeredDown(BTN_A))
                rigidbody->AddForce(SVec3(0.0f, controller->jumpSpeed, 0.0f));
        }
    }
}
