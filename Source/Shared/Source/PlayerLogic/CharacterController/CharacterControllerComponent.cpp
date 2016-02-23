/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CharacterControllerComponent.cpp
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "CharacterControllerComponent.h"
#include "GameEvents.h"
#include <Application.h>

NATIVE_COMPONENT_DEFINITION( CharacterController );

CharacterController::CharacterController(void)
    : BaseComponent( )
    , m_rotateSpeed( 4.0f )
    , m_jump( false ) { }

CharacterController::~CharacterController(void)
{
    GetOwner( )->Listener( this )
        .Off(game::MOVEMENT_COMMAND, &CharacterController::SetMoveDirection)
        .Off( game::LOOK_COMMAND, &CharacterController::SetLookDirection )
        .Off( game::JUMP_COMMAND, &CharacterController::Jump );
}

float CharacterController::GetRotateSpeed(void) const
{
    return m_rotateSpeed;
}

void CharacterController::SetRotateSpeed(float rotateSpeed)
{
    if (rotateSpeed < 0.0f)
        return;

    m_rotateSpeed = rotateSpeed;
}

const ursine::Vec2& CharacterController::GetMoveDirection(void) const
{
    return m_moveDir;
}

void CharacterController::SetMoveDirection(const ursine::Vec2& moveDir)
{
    m_moveDir = moveDir;
}

const ursine::Vec2& CharacterController::GetLookDirection(void) const
{
    return m_lookDir;
}

void CharacterController::SetLookDirection(const ursine::Vec2& lookDir)
{
    m_lookDir = lookDir;
}

void CharacterController::OnInitialize(void)
{
    GetOwner( )->Listener(this)
        .On( game::MOVEMENT_COMMAND, &CharacterController::SetMoveDirection)
        .On( game::LOOK_COMMAND, &CharacterController::SetLookDirection )
        .On( game::JUMP_COMMAND, &CharacterController::Jump );
}

void CharacterController::SetMoveDirection(EVENT_HANDLER(game::LOOK_COMMAND))
{
    EVENT_ATTRS(ursine::ecs::Entity, game::MovementEventArgs);

    m_moveDir = args->m_moveDir;
}

void CharacterController::SetLookDirection(EVENT_HANDLER(game::LOOK_COMMAND))
{
    EVENT_ATTRS(ursine::ecs::Entity, game::MovementEventArgs);

    m_lookDir = args->m_moveDir;
}

void CharacterController::Jump(EVENT_HANDLER(game::JUMP_COMMAND))
{
    m_jump = true;
}
