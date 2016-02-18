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
#include <SweptControllerComponent.h>

using namespace ursine;
using namespace ursine::ecs;

ENTITY_SYSTEM_DEFINITION(CharacterControllerSystem);

CharacterControllerSystem::CharacterControllerSystem(ursine::ecs::World *world)
    : FilterSystem( world, Filter( ).All<Rigidbody, CharacterController>( ) )
{
    
}

void CharacterControllerSystem::Process(Entity *entity)
{
    auto *controller = entity->GetComponent<CharacterController>( );
    auto *swept = entity->GetComponent<SweptController>( );
    auto rotateSpeed = controller->GetRotateSpeed( );

    auto transform = entity->GetTransform( );
    auto rigidbody = entity->GetComponent<Rigidbody>( );
    
    auto lookDir = controller->GetLookDirection( );
    
    auto child = transform->GetChild(0);
    auto cam = transform->GetComponentInChildren<Camera>( );

    // This is an immidiate fix, cause fuck eet. - Jordan
    rigidbody->SetGravity( SVec3( 0.0f, -100.0f, 0.0f ) );
    
    // Looking logic
    if (lookDir.Length( ) > controller->m_deadZone)
    {
        auto lookAngle = lookDir * rotateSpeed;

        if ( cam )
        {
            auto camTrans = cam->GetOwner( )->GetTransform( );

            // is this actually doing what we want?
            auto look = camTrans->GetForward( );

            if ( lookAngle.Y( ) < 0.0f )
            {
                // look down
                if ( look.Y( ) > -0.75f )
                {
                    look = SQuat(-lookAngle.Y( ), camTrans->GetRight( )) * look;

                    if ( look.Y( ) < -0.75 )
                        look.Y( ) = -0.75;
                }
            }
            else if ( lookAngle.Y( ) > 0.0f )
            {
                // look up
                if ( look.Y( ) < 0.75f )
                {
                    look = SQuat(-lookAngle.Y( ), camTrans->GetRight( )) * look;

                    if ( look.Y( ) > 0.75 )
                        look.Y( ) = 0.75;
                }
            }

            camTrans->LookAt(camTrans->GetWorldPosition( ) + look);
        }

        transform->SetWorldRotation(
            transform->GetWorldRotation( ) *
            SQuat(0.0f, lookAngle.X( ), 0.0f)
            );
    }

    auto move = controller->GetMoveDirection( );

    auto forward = transform->GetForward( ) * move.Y( );
    auto strafe = transform->GetRight( ) * move.X( );
    auto accum = forward + strafe;

    if (controller->m_jump)
    {
        swept->Jump( );
        controller->m_jump = false;
    }

    swept->SetMovementDirection({ accum.X( ), 0.0f, accum.Z( ) });

    // Reset the look direction
    controller->SetLookDirection( Vec2::Zero( ) );
    controller->SetMoveDirection( Vec2::Zero( ) );
}
