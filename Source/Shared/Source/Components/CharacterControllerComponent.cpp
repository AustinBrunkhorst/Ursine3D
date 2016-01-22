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
    , m_moveSpeed( 50.0f )
    , m_rotateSpeed( 4.0f )
    , m_jumpSpeed( 50.0f )
    , m_jumpInterval( 1.0f )
    , m_jumpTimer( 1.0f )
	, m_jump( false ) { }

CharacterController::~CharacterController(void)
{
    GetOwner( )->GetWorld( )->Listener(this)
        .Off(ursine::ecs::WORLD_UPDATE, &CharacterController::onUpdate);

    GetOwner( )->Listener( this )
        .Off( game::LOOK_COMMAND, &CharacterController::SetLookDirection )
        .Off( game::JUMP_COMMAND, &CharacterController::Jump );
}

float CharacterController::GetMoveSpeed(void) const
{
    return m_moveSpeed;
}

void CharacterController::SetMoveSpeed(float moveSpeed)
{
    if (moveSpeed < 0.0f)
        return;

    m_moveSpeed = moveSpeed;
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

float CharacterController::GetJumpSpeed(void) const
{
    return m_jumpSpeed;
}

void CharacterController::SetJumpSpeed(float jumpSpeed)
{
    if (jumpSpeed < 0.0f)
        return;

    m_jumpSpeed = jumpSpeed;
}

float CharacterController::GetJumpInterval(void) const
{
    return m_jumpInterval;
}

void CharacterController::SetJumpInterval(float jumpInterval)
{
    if (jumpInterval < 0.0f)
        return;

    m_jumpInterval = jumpInterval;
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
    Component::OnInitialize( );

    GetOwner( )->GetWorld( )->Listener( this )
        .On( ursine::ecs::WORLD_UPDATE, &CharacterController::onUpdate );

    GetOwner( )->Listener(this)
        .On( game::LOOK_COMMAND, &CharacterController::SetLookDirection )
        .On( game::JUMP_COMMAND, &CharacterController::Jump );
}

void CharacterController::onUpdate(EVENT_HANDLER(World))
{
    // Increment the timer
    if (m_jumpTimer <= m_jumpInterval)
        m_jumpTimer += ursine::Application::Instance->GetDeltaTime( );
}

void CharacterController::SetLookDirection(EVENT_HANDLER(game::LOOK_COMMAND))
{
    EVENT_ATTRS(ursine::ecs::Entity, game::MovementEventArgs);

    m_lookDir = args->m_moveDir;
}

void CharacterController::Jump(EVENT_HANDLER(game::JUMP_COMMAND))
{
    if ( m_jumpTimer >= m_jumpInterval )
    {
        m_jump = true;
        m_jumpTimer = 0.0f;
    }
}
