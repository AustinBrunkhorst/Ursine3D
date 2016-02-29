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

#include <SystemManager.h>
#include <DebugSystem.h>

NATIVE_COMPONENT_DEFINITION( BossVineAI );

using namespace ursine;
using namespace ecs;

BossVineAI::BossVineAI(void)
    : BaseComponent( )
    , m_faceClosestPlayer( true )
    , m_turnSpeed( 90.0f )
    , m_angle( 45.0f )
    , m_range( 5.0f )
    , m_duration( 0.5f )
    , m_damage( 10.0f )
    , m_cooldown( 2.0f )
    , m_stateMachine( this )
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

float BossVineAI::GetTurnSpeed(void) const
{
    return m_turnSpeed;
}

void BossVineAI::SetTurnSpeed(float speed)
{
    m_turnSpeed = speed;
}

float BossVineAI::GetWhipRange(void) const
{
    return m_range;
}

void BossVineAI::SetWhipRange(float range)
{
    m_range = range;
}

float BossVineAI::GetWhipAngle(void) const
{
    return m_angle;
}

void BossVineAI::SetWhipAngle(float angle)
{
    m_angle = angle;
}

float BossVineAI::GetWhipDuration(void) const
{
    return m_duration;
}

void BossVineAI::SetWhipDuration(float duration)
{
    m_duration = duration;
}

float BossVineAI::GetWhipDamage(void) const
{
    return m_damage;
}

void BossVineAI::SetWhipDamage(float damage)
{
    m_damage = damage;
}

float BossVineAI::GetWhipCooldown(void) const
{
    return m_cooldown;
}

void BossVineAI::SetWhipCooldown(float cooldown)
{
    m_cooldown = cooldown;
}

void BossVineAI::OnInitialize(void)
{
    GetOwner( )->GetWorld( )->Listener( this )
        .On( WORLD_UPDATE, &BossVineAI::onUpdate );

    // Setup the state machien
    auto lookState = m_stateMachine.AddState<VineLookForInRangePlayersState>( );

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

    drawer->DrawCircle( center, normal, m_range, Color::Yellow, 5.0f, true );
}

#endif
