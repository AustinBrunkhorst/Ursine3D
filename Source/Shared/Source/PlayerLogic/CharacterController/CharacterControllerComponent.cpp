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

using namespace ursine;
using namespace ecs;
using namespace resources;

CharacterController::CharacterController(void)
    : BaseComponent( )
    , m_rotateSpeed( 4.0f )
    , m_rotationModifier( 1.0f )
    , m_jump( false )
    , m_jumping( false )
    , m_deadZone( 0.0f )
    , m_deadZoneSnap( 0.0f )
    , m_mediumRotationFloor( 0.0f )
    , m_highRotationFloor( 1.0f )
    , m_lowRotationFactor( 0.0f )
    , m_mediumRotationFactor( 1.0f )
    , m_jumpDirectionScalar( 0.2f )
    , m_lockMovement( false )
    , m_lockLooking( false )
    , m_stepInterval( 1.0f )
    , m_stepTimer( 0.1f ) { }

CharacterController::~CharacterController(void)
{
    GetOwner( )->Listener( this )
        .Off( game::MOVEMENT_COMMAND, &CharacterController::SetMoveDirection)
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

float CharacterController::GetRotationModifier(void) const
{
    return m_rotationModifier;
}

void CharacterController::SetRotationModifier(float modifier)
{
    m_rotationModifier = modifier;
}

float CharacterController::GetDeadZone(void) const
{
	return m_deadZone;
}

void CharacterController::SetDeadZone(float deadZone)
{
	m_deadZone = deadZone;
}

float CharacterController::GetDeadZoneSnap(void) const
{
    return m_deadZoneSnap;
}

void CharacterController::SetDeadZoneSnap(float snap)
{
    m_deadZoneSnap = snap;
}

float CharacterController::GetMediumRotationFloor(void) const
{
	return m_mediumRotationFloor;
}

void CharacterController::SetMediumRotationFloor(float floor)
{
	m_mediumRotationFloor = floor;
}

float CharacterController::GetHighRotationFloor(void) const
{
	return m_highRotationFloor;
}

void CharacterController::SetHighRotationFloor(float floor)
{
	m_highRotationFloor = floor;
}

float CharacterController::GetLowRotationFactor(void) const
{
	return m_lowRotationFactor;
}

void CharacterController::SetLowRotationFactor(float rotationFactor)
{
	m_lowRotationFactor = rotationFactor;
}

float CharacterController::GetMediumRotationFactor(void) const
{
	return m_mediumRotationFactor;
}

void CharacterController::SetMediumRotationFactor(float rotationFactor)
{
	m_mediumRotationFactor = rotationFactor;
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

bool CharacterController::GetLockMovement(void) const
{
    return m_lockMovement;
}

void CharacterController::SetLockMovement(bool flag)
{
    m_lockMovement = flag;
}

bool CharacterController::GetLockLooking(void) const
{
    return m_lockLooking;
}

void CharacterController::SetLockLooking(bool flag)
{
    m_lockLooking = flag;

    if (m_lockLooking)
        GetOwner( )->Dispatch( game::CEASE_FIRE, nullptr );
    else
        GetOwner( )->Dispatch( game::FIRE_AT_WILL, nullptr );
}

float CharacterController::GetJumpDirectionScalar(void) const
{
    return m_jumpDirectionScalar;
}

void CharacterController::SetJumpDirectionScalar(float scalar)
{
    m_jumpDirectionScalar = scalar;

    NOTIFY_COMPONENT_CHANGED( "jumpDirectionScalar", m_jumpDirectionScalar );
}

const ResourceReference &CharacterController::GetFootSfx(void) const
{
    return m_footSfx;
}

void CharacterController::SetFootSfx(const ResourceReference &audioEvent)
{
    m_footSfx = audioEvent;

    NOTIFY_COMPONENT_CHANGED( "footSfx", m_footSfx );
}

const std::string &CharacterController::GetFootStepEmitterEntityName(void) const
{
    return m_footStepEntityName;
}

void CharacterController::SetFootStepEmitterEntityName(const std::string &entityName)
{
    m_footStepEntityName = entityName;

    NOTIFY_COMPONENT_CHANGED( "footStepEmitterEntityName", m_footStepEntityName );
}

EntityHandle CharacterController::GetFootStepEntity(void)
{
    return GetOwner( )->GetChildByName( m_footStepEntityName );
}

float CharacterController::GetStepSfxInterval(void) const
{
    return m_stepInterval;
}

void CharacterController::SetStepSfxInterval(float interval)
{
    m_stepInterval = interval;

    NOTIFY_COMPONENT_CHANGED( "stepSfxInterval", m_stepInterval );
}

const ResourceReference &CharacterController::GetJumpSfx(void) const
{
    return m_jumpSfx;
}

void CharacterController::SetJumpSfx(const ResourceReference &sfx)
{
    m_jumpSfx = sfx;

    NOTIFY_COMPONENT_CHANGED( "jumpSfx", m_jumpSfx );
}

const ResourceReference &CharacterController::GetLandSfx(void) const
{
    return m_landSfx;
}

void CharacterController::SetLandSfx(const ResourceReference &sfx)
{
    m_landSfx = sfx;

    NOTIFY_COMPONENT_CHANGED( "landSfx", m_landSfx );
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
