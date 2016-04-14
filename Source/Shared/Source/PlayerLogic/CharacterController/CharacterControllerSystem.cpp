/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CharacterControllerSystem.cpp
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "CharacterControllerSystem.h"
#include "CharacterControllerComponent.h"

#include "RigidbodyComponent.h"

#include <CameraComponent.h>
#include <AudioEmitterComponent.h>
#include <SweptControllerComponent.h>

#include "AudioManager.h"

using namespace ursine;
using namespace ursine::ecs;

ENTITY_SYSTEM_DEFINITION(CharacterControllerSystem);

CharacterControllerSystem::CharacterControllerSystem(ursine::ecs::World *world)
    : FilterSystem( world, Filter( ).All<Rigidbody, CharacterController>( ) )
{
    
}

void CharacterControllerSystem::Process(const ursine::ecs::EntityHandle &entity)
{
    auto *controller = entity->GetComponent<CharacterController>( );
    auto *swept = entity->GetComponent<SweptController>( );
    auto rotateSpeed = controller->GetRotateSpeed( );
    auto rotationModifier = controller->GetRotationModifier( );

    auto transform = entity->GetTransform( );
    auto rigidbody = entity->GetComponent<Rigidbody>( );
    
    auto lookDir = controller->GetLookDirection( );
    
    auto child = transform->GetChild(0);
    auto cam = transform->GetComponentInChildren<Camera>( );

    auto mediumRotationFloor = controller->GetMediumRotationFloor( );
    auto highRotationFloor = controller->GetHighRotationFloor( );
    auto lowRotationFactor = controller->GetLowRotationFactor( );
    auto mediumRotationFactor = controller->GetMediumRotationFactor( );

    // This is an immidiate fix, cause fuck eet. - Jordan
    rigidbody->SetGravity( SVec3( 0.0f, -100.0f, 0.0f ) );
    
    // Looking logic
    if (lookDir.Length( ) > controller->m_deadZone && !controller->m_lockLooking)
    {
        if (abs(lookDir.X( )) < controller->m_deadZoneSnap)
            lookDir.X( ) = 0.0f;

        if (abs(lookDir.Y( )) < controller->m_deadZoneSnap)
            lookDir.Y( ) = 0.0f;

        // Rotation Zones & Factors Modification
        if (lookDir.Length() < mediumRotationFloor)
            lookDir = lookDir * lowRotationFactor;
        else if (lookDir.Length( ) < highRotationFloor)
            lookDir = lookDir * mediumRotationFactor;

        auto lookAngle = lookDir * rotateSpeed * rotationModifier;

        if (cam)
        {
            auto camTrans = cam->GetOwner( )->GetTransform( );

            // is this actually doing what we want?
            auto look = camTrans->GetForward( );

            if (lookAngle.Y( ) < 0.0f)
            {
                // look down
                if (look.Y( ) > -0.95f)
                {
                    look = SQuat(-lookAngle.Y( ), camTrans->GetRight( )) * look;

                    if (look.Y( ) < -0.95f)
                        look.Y( ) = -0.95f;
                }
            }
            else if (lookAngle.Y( ) > 0.0f)
            {
                if (look.Y( ) < 0.95f)
                {
                    // look up
                    look = SQuat(-lookAngle.Y( ), camTrans->GetRight( )) * look;

                    if (look.Y( ) > 0.95f)
                        look.Y( ) = 0.95f;
                }
            }

            camTrans->LookAt(camTrans->GetWorldPosition( ) + look);
        }

        transform->SetWorldRotation(
            transform->GetWorldRotation( ) *
            SQuat(0.0f, lookAngle.X( ), 0.0f)
        );
    }

    if (!controller->m_lockMovement)
    {
        auto move = controller->GetMoveDirection( );

        auto forward = transform->GetForward( ) * move.Y( );
        auto strafe = transform->GetRight( ) * move.X( );
        auto accum = forward + strafe;

        accum.Y( ) = 0.0f;

        if (controller->m_jumping && swept->GetGrounded( ))
        {
            // play the sound effect
            controller->GetFootStepEntity( )->GetComponent<AudioEmitter>( )
                ->PushEvent( controller->GetLandSfx( ) );

            controller->m_jumping = false;
        }

        if (controller->m_jump)
        {
            if (swept->GetGrounded( ))
            {
                auto currVel = swept->GetControllerVelocity( );

                swept->JumpDirectionally( currVel * ( 1.0f + controller->GetJumpDirectionScalar( ) ) );

                // play the sound effect
                controller->GetFootStepEntity( )->GetComponent<AudioEmitter>( )
                    ->PushEvent( controller->GetJumpSfx( ) );

                controller->m_jumping = true;
            }

            controller->m_jump = false;
        }

        swept->SetMovementDirection( accum );

        if (accum.Length( ) > math::Epsilon && swept->GetGrounded( ))
        {
            if (controller->m_stepTimer <= 0.0f)
            {
                // play audio event
                controller->GetFootStepEntity( )->GetComponent<AudioEmitter>( )->PushEvent(
                    controller->m_footSfx
                );

                controller->m_stepTimer = controller->m_stepInterval;
            }
            else
            {
                controller->m_stepTimer -= Application::Instance->GetDeltaTime( );
            }
        }
    }

    // Reset the look direction
    controller->SetLookDirection( Vec2::Zero( ) );
    controller->SetMoveDirection( Vec2::Zero( ) );
}
