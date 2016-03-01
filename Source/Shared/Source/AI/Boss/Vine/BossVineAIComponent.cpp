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

#include "BossVineAiComponent.h"

#include "BossVineStateMachine.h"
#include "BossVineState.h"
#include "VineLookForInRangePlayersState.h"
#include "VineWhipState.h"
#include "VineUprootState.h"

#include <FloatCondition.h>

#include <SystemManager.h>
#include <DebugSystem.h>

NATIVE_COMPONENT_DEFINITION( BossVineAI );

using namespace ursine;
using namespace ecs;

BossVineAI::BossVineAI(void)
    : BaseComponent( )
    , m_faceClosestPlayer( true )
    , m_whipTurnSpeed( 90.0f )
    , m_whipAngle( 45.0f )
    , m_whipRange( 5.0f )
    , m_whipDuration( 0.5f )
    , m_whipCooldown( 2.0f )
    , m_digSpeed( 2.0f )
    , m_digTurnSpeed( 2.0f )
    , m_uprootDelay( 2.0f )
    , m_colliderSize( 1.0f, 1.0f, 1.0f )
    , m_stateMachine( this )
    , m_animator( nullptr )
    , m_target( nullptr )
{
}

BossVineAI::~BossVineAI(void)
{
    GetOwner( )->GetWorld( )->Listener( this )
        .Off( WORLD_UPDATE, &BossVineAI::onUpdate );
}

bool BossVineAI::GetFaceClosestPlayer(void) const
{
    return m_faceClosestPlayer;
}

void BossVineAI::SetFaceClosestPlayer(bool flag)
{
    m_faceClosestPlayer = flag;
}

float BossVineAI::GetWhipTurnSpeed(void) const
{
    return m_whipTurnSpeed;
}

void BossVineAI::SetWhipTurnSpeed(float turnSpeed)
{
    m_whipTurnSpeed = turnSpeed;
}

float BossVineAI::GetWhipRange(void) const
{
    return m_whipRange;
}

void BossVineAI::SetWhipRange(float range)
{
    m_whipRange = range;
}

float BossVineAI::GetWhipAngle(void) const
{
    return m_whipAngle;
}

void BossVineAI::SetWhipAngle(float angle)
{
    m_whipAngle = angle;
}

float BossVineAI::GetWhipDuration(void) const
{
    return m_whipDuration;
}

void BossVineAI::SetWhipDuration(float duration)
{
    m_whipDuration = duration;
}

float BossVineAI::GetWhipCooldown(void) const
{
    return m_whipCooldown;
}

void BossVineAI::SetWhipCooldown(float cooldown)
{
    m_whipCooldown = cooldown;
}

float BossVineAI::GetDigSpeed(void) const
{
    return m_digSpeed;
}

void BossVineAI::SetDigSpeed(float digSpeed)
{
    m_digSpeed = digSpeed;
}

float BossVineAI::GetDigTurnSpeed(void) const
{
    return m_digTurnSpeed;
}

void BossVineAI::SetDigTurnSpeed(float turnSpeed)
{
    m_digTurnSpeed = turnSpeed;
}

float BossVineAI::GetUprootDelay(void) const
{
    return m_uprootDelay;
}

void BossVineAI::SetUprootDelay(float delay)
{
    m_uprootDelay = delay;
}

const SVec3 &BossVineAI::GetColliderSize(void) const
{
    return m_colliderSize;
}

void BossVineAI::SetColliderSize(const SVec3 &colliderSize)
{
    m_colliderSize = colliderSize;
}

EntityAnimator *BossVineAI::GetAnimator(void)
{
    if (!m_animator)
        m_animator = GetOwner( )->GetComponentInChildren<EntityAnimator>( );

    return m_animator;
}

Entity *BossVineAI::GetTarget(void)
{
    return m_target;
}

void BossVineAI::SetTarget(Entity *target)
{
    m_target = target;
}

void BossVineAI::OnInitialize(void)
{
    GetOwner( )->GetWorld( )->Listener( this )
        .On( WORLD_UPDATE, &BossVineAI::onUpdate );

    // Setup the state machine
    auto lookState = m_stateMachine.AddState<VineLookForInRangePlayersState>( );
    auto whipState = m_stateMachine.AddState<VineWhipState>( );
    auto uprootState = m_stateMachine.AddState<VineUprootState>( );

    auto trans = lookState->AddTransition( whipState, "To Whip" );
    trans->AddCondition<sm::FloatCondition>( "Cooldown", sm::Comparison::LessThan, 0.0f );

    whipState->AddTransition( uprootState, "To Look" );

    m_stateMachine.SetInitialState( lookState );
}

void BossVineAI::onUpdate(EVENT_HANDLER(World))
{
    // update the state machine
    m_stateMachine.Update( );
}

#if defined(URSINE_WITH_EDITOR)

void BossVineAI::drawRange(void)
{
    auto drawer = GetOwner( )->GetWorld( )->GetEntitySystem<DebugSystem>( );

    auto trans = GetOwner( )->GetTransform( );
    auto center = trans->GetWorldPosition( );
    auto normal = trans->GetUp( );

    drawer->DrawCircle( center, normal, m_whipRange, Color::Yellow, 5.0f, true );
}

#endif
