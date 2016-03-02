/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossVineAIComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "VineAIComponent.h"

#include "VineAIStateMachine.h"
#include "VineLookForInRangePlayersState.h"
#include "VineWhipState.h"
#include "VineUprootState.h"

#include <FloatCondition.h>
#include <BoolCondition.h>

#include <SystemManager.h>
#include <DebugSystem.h>
#include <EntityEvent.h>

NATIVE_COMPONENT_DEFINITION( VineAI );

using namespace ursine;
using namespace ecs;

VineAI::VineAI(void)
    : BaseComponent( )
    , m_faceClosestPlayer( true )
    , m_whipTurnSpeed( 90.0f )
    , m_whipAngle( 45.0f )
    , m_whipRange( 5.0f )
    , m_whipDuration( 0.5f )
    , m_whipCooldown( 2.0f )
    , m_digSpeed( 2.0f )
    , m_digTurnSpeed( 2.0f )
    , m_uprootDistance( 2.0f )
    , m_uprootDelay( 2.0f )
    , m_uprootCooldown( 5.0f )
    , m_colliderSize( 1.0f, 1.0f, 1.0f )
    , m_stateMachine( this )
    , m_animator( nullptr )
    , m_target( nullptr )
{
}

VineAI::~VineAI(void)
{
    GetOwner( )->GetWorld( )->Listener( this )
        .Off( WORLD_UPDATE, &VineAI::onUpdate );
}

bool VineAI::GetFaceClosestPlayer(void) const
{
    return m_faceClosestPlayer;
}

void VineAI::SetFaceClosestPlayer(bool flag)
{
    m_faceClosestPlayer = flag;
}

float VineAI::GetWhipTurnSpeed(void) const
{
    return m_whipTurnSpeed;
}

void VineAI::SetWhipTurnSpeed(float turnSpeed)
{
    m_whipTurnSpeed = turnSpeed;
}

float VineAI::GetWhipRange(void) const
{
    return m_whipRange;
}

void VineAI::SetWhipRange(float range)
{
    m_whipRange = range;
}

float VineAI::GetWhipAngle(void) const
{
    return m_whipAngle;
}

void VineAI::SetWhipAngle(float angle)
{
    m_whipAngle = angle;
}

float VineAI::GetWhipDuration(void) const
{
    return m_whipDuration;
}

void VineAI::SetWhipDuration(float duration)
{
    m_whipDuration = duration;
}

float VineAI::GetWhipCooldown(void) const
{
    return m_whipCooldown;
}

void VineAI::SetWhipCooldown(float cooldown)
{
    m_whipCooldown = cooldown;
}

float VineAI::GetDigSpeed(void) const
{
    return m_digSpeed;
}

void VineAI::SetDigSpeed(float digSpeed)
{
    m_digSpeed = digSpeed;
}

float VineAI::GetDigTurnSpeed(void) const
{
    return m_digTurnSpeed;
}

void VineAI::SetDigTurnSpeed(float turnSpeed)
{
    m_digTurnSpeed = turnSpeed;
}

const std::string &VineAI::GetDigParticleEmitterName(void) const
{
    return m_digParticleEmitterName;
}

void VineAI::SetDigParticleEmitterName(const std::string &name)
{
    m_digParticleEmitterName = name;
}

float VineAI::GetUprootDistance(void) const
{
    return m_uprootDistance;
}

void VineAI::SetUprootDistance(float distance)
{
    m_uprootDistance = distance;
}

float VineAI::GetUprootDelay(void) const
{
    return m_uprootDelay;
}

void VineAI::SetUprootDelay(float delay)
{
    m_uprootDelay = delay;
}

float VineAI::GetUprootCooldown(void) const
{
    return m_uprootCooldown;
}

void VineAI::SetUprootCooldown(float cooldown)
{
    m_uprootCooldown = cooldown;
}

const SVec3 &VineAI::GetColliderSize(void) const
{
    return m_colliderSize;
}

void VineAI::SetColliderSize(const SVec3 &colliderSize)
{
    m_colliderSize = colliderSize;
}

EntityAnimator *VineAI::GetAnimator(void)
{
    return m_animator;
}

Entity *VineAI::GetTarget(void)
{
    return m_target;
}

void VineAI::SetTarget(Entity *target)
{
    m_target = target;
}

void VineAI::OnInitialize(void)
{
    // it spawns, comes up from ground, and is able to whip.
    // A vine can then be set to be a uproot vine.  That would mean that it would
    // go and chase a target.

    GetOwner( )->GetWorld( )->Listener( this )
        .On( WORLD_UPDATE, &VineAI::onUpdate );

    // Setup the state machine
    auto lookState = m_stateMachine.AddState<VineLookForInRangePlayersState>( );
    auto whipState = m_stateMachine.AddState<VineWhipState>( );
    auto uprootState = m_stateMachine.AddState<VineUprootState>( );

    auto trans = lookState->AddTransition( whipState, "To Whip" );

    // The cooldown must be up
    trans->AddCondition<sm::FloatCondition>( 
        VineAIStateMachine::WhipCooldown, 
        sm::Comparison::LessThan, 0.0f 
    );

    // The target must be in range
    trans->AddCondition<sm::BoolCondition>(
        VineAIStateMachine::InRange, true
    );

    // The target must be in view
    trans->AddCondition<sm::BoolCondition>(
        VineAIStateMachine::InView, true
    );

    whipState->AddTransition( lookState, "To Look" );

    trans = lookState->AddTransition( uprootState, "To Uproot" );

    trans->AddCondition<sm::FloatCondition>(
        VineAIStateMachine::UprootCooldown,
        sm::Comparison::LessThan, 0.0f
    );

    uprootState->AddTransition( lookState, "To Look" );

    m_stateMachine.SetInitialState( lookState );

    GetOwner( )->Listener( this )
        .On( ENTITY_HIERARCHY_SERIALIZED, &VineAI::onChildrenSerialized );
}

void VineAI::onUpdate(EVENT_HANDLER(World))
{
    // update the state machine
    m_stateMachine.Update( );
}

void VineAI::onChildrenSerialized(EVENT_HANDLER(Entity))
{
    m_animator = GetOwner( )->GetComponentInChildren<EntityAnimator>( );

    GetOwner( )->Listener( this )
        .Off( ENTITY_HIERARCHY_SERIALIZED, &VineAI::onChildrenSerialized );
}

#if defined(URSINE_WITH_EDITOR)

void VineAI::drawRange(void)
{
    auto drawer = GetOwner( )->GetWorld( )->GetEntitySystem<DebugSystem>( );

    auto trans = GetOwner( )->GetTransform( );
    auto center = trans->GetWorldPosition( );
    auto normal = trans->GetUp( );

    drawer->DrawCircle( center, normal, m_whipRange, Color::Yellow, 5.0f, true );
}

#endif
