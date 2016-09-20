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
#include "VineSpawnState.h"
#include "VineLookForInRangePlayersState.h"
#include "VineWhipState.h"
#include "VineUprootState.h"
#include "VineGoHomeState.h"
#include "VineDeathState.h"

#include <FloatCondition.h>
#include <BoolCondition.h>

#include <SystemManager.h>
#include <DebugSystem.h>
#include <EntityEvent.h>
#include <Application.h>

NATIVE_COMPONENT_DEFINITION( VineAI );

using namespace ursine;
using namespace ecs;
using namespace resources;

VineAI::VineAI(void)
    : BaseComponent( )
    , EventDispatcher( this )
    , m_faceClosestPlayer( true )
    , m_whipTurnSpeed( 90.0f )
    , m_whipAngle( 45.0f )
    , m_whipRange( 5.0f )
    , m_whipDuration( 0.5f )
    , m_whipCooldown( 2.0f )
    , m_digSpeed( 2.0f )
    , m_digRayStartY( -20.0f )
    , m_digTurnSpeed( 2.0f )
    , m_uprootDistance( 2.0f )
    , m_uprootDelay( 2.0f )
    , m_uprootCooldown( 5.0f )
    , m_stateMachine( this )
    , m_animator( nullptr )
    , m_target( nullptr )
    , m_timeOfLastPursue( Application::Instance->GetTimeSinceStartup( ) )
    , m_currentHealthThreshold( 0 ) { }

VineAI::~VineAI(void)
{
    GetOwner( )->GetWorld( )->Listener( this )
        .Off( WORLD_UPDATE, &VineAI::onUpdate );

    auto health = GetOwner( )->GetComponent<Health>( );

    if (health)
    {
        health->Listener( this )
            .Off( HEALTH_DAMAGE_TAKEN, &VineAI::onDamageTaken );
    }
}

void VineAI::OnSceneReady(ursine::Scene *scene)
{
    auto health = GetOwner( )->GetComponent<Health>( );

    if (health)
    {
        health->Listener( this )
            .On( HEALTH_DAMAGE_TAKEN, &VineAI::onDamageTaken );
    }
}

const std::string &VineAI::GetSpawnParticleEmitterName(void) const
{
    return m_spawnParticleEmitterName;
}

void VineAI::SetSpawnParticleEmitterName(const std::string &name)
{
    m_spawnParticleEmitterName = name;

    NOTIFY_COMPONENT_CHANGED( "spawnParticleEmitterName", m_spawnParticleEmitterName );
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

float VineAI::GetDigRayStartY(void) const
{
    return m_digRayStartY;
}

void VineAI::SetDigRayStartY(float y)
{
    m_digRayStartY = y;

    NOTIFY_COMPONENT_CHANGED( "digRayStartY", m_digRayStartY );
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

const ResourceReference &VineAI::GetSwipeSfx(void) const
{
    return m_swipeSfx;
}

void VineAI::SetSwipeSfx(const ResourceReference &swipeSfx)
{
    m_swipeSfx = swipeSfx;

    NOTIFY_COMPONENT_CHANGED( "swipeSfx", m_swipeSfx );
}

const ResourceReference &VineAI::GetBurrowSfx(void) const
{
    return m_burrowSfx;
}

void VineAI::SetBurrowSfx(const ResourceReference &burrowSfx)
{
    m_burrowSfx = burrowSfx;

    NOTIFY_COMPONENT_CHANGED( "burrowSfx", m_burrowSfx );
}

const ResourceReference &VineAI::GetEmergeSfx(void) const
{
    return m_emergeSfx;
}

const ResourceReference &VineAI::GetDeathSfx(void) const
{
    return m_deathSfx;
}

void VineAI::SetDeathSfx(const ResourceReference &deathSfx)
{
    m_deathSfx = deathSfx;

    NOTIFY_COMPONENT_CHANGED( "deathSfx", m_deathSfx );
}

void VineAI::SetEmergeSfx(const ResourceReference &emergeSfx)
{
    m_emergeSfx = emergeSfx;

    NOTIFY_COMPONENT_CHANGED( "emergeSfx", m_emergeSfx );
}

Animator *VineAI::GetAnimator(void)
{
    return m_animator;
}

EntityHandle VineAI::GetTarget(void)
{
    return m_target;
}

void VineAI::SetTarget(EntityHandle target)
{
    m_target = target;
}

void VineAI::SetHomeLocation(const SVec3 &homeLocation)
{
    m_homeLocation = homeLocation;
}

const SVec3 &VineAI::GetHomeLocation(void) const
{
    return m_homeLocation;
}

void VineAI::GoToHomeLocation(void)
{
    m_stateMachine.SetBool( VineAIStateMachine::GoHome, true );
    m_stateMachine.SetBool( VineAIStateMachine::PursueTarget, false );
}

void VineAI::JumpToHomeLocation(void)
{
    m_stateMachine.SetBool( VineAIStateMachine::GoHome, true );
    m_stateMachine.SetBool( VineAIStateMachine::JumpToHome, true );
    m_stateMachine.SetBool( VineAIStateMachine::PursueTarget, false );
}

bool VineAI::IsHome(void)
{
    return m_stateMachine.GetBool( VineAIStateMachine::IsHome );
}

void VineAI::PursueTarget(void)
{
    m_stateMachine.SetBool( VineAIStateMachine::PursueTarget, true );
    m_stateMachine.SetBool( VineAIStateMachine::IsHome, false );
    m_timeOfLastPursue = Application::Instance->GetTimeSinceStartup( );
}

const TimeSpan &VineAI::GetTimeOfLastPursue(void) const
{
    return m_timeOfLastPursue;
}

void VineAI::OnInitialize(void)
{
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
    GetOwner( )->Listener( this )
        .Off( ENTITY_HIERARCHY_SERIALIZED, &VineAI::onChildrenSerialized );

    m_animator = GetOwner( )->GetComponentInChildren<Animator>( );

    // Setup the state machine
    m_stateMachine.Initialize( );

    auto spawnState = m_stateMachine.AddState<VineSpawnState>( );
    auto lookState = m_stateMachine.AddState<VineLookForInRangePlayersState>( );
    auto whipState = m_stateMachine.AddState<VineWhipState>( );
    auto uprootState = m_stateMachine.AddState<VineUprootState>( );
    auto goHomeState = m_stateMachine.AddState<VineGoHomeState>( );
    auto deathState = m_stateMachine.AddState<VineDeathState>( );

    spawnState->AddTransition( lookState, "To Look" );

    // Setup the transition to the Whip ability
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

    // Setup the transition to the uproot ability
    trans = lookState->AddTransition( uprootState, "To Uproot" );

    // The cooldown must be up
    trans->AddCondition<sm::FloatCondition>(
        VineAIStateMachine::UprootCooldown,
        sm::Comparison::LessThan, 0.0f
    );

    // We must be set to pursue our target
    trans->AddCondition<sm::BoolCondition>(
        VineAIStateMachine::PursueTarget,
        true
    );

    uprootState->AddTransition( lookState, "To Look" );

    // Setup the transition to the back to home state
    trans = lookState->AddTransition( goHomeState, "To Home" );

    // We must be set to go back home
    trans->AddCondition<sm::BoolCondition>(
        VineAIStateMachine::GoHome,
        true
    );

    goHomeState->AddTransition( lookState, "To Look" );

    // Tell the looking state to go to the death state when it happens
    trans = lookState->AddTransition( deathState, "To Death" );

    trans->AddCondition<sm::BoolCondition>(
        VineAIStateMachine::Dead,
        true
    );

    m_stateMachine.SetInitialState( spawnState );

    GetOwner( )->GetWorld( )->Listener( this )
        .On( WORLD_UPDATE, &VineAI::onUpdate );
}

void VineAI::onDamageTaken(EVENT_HANDLER(Health))
{
    EVENT_ATTRS(Health, HealthEventArgs);

    if (healthThresholds.Size( ) <= m_currentHealthThreshold)
        return;

    auto &current = healthThresholds[ m_currentHealthThreshold ];

    if (args->percentage <= current.percentage)
    {
        // Dispatch an event letting everyone know a current threshold was hit
        Dispatch( VINE_HEALTH_THRESHOLD_REACHED, nullptr );

        // increment our index
        ++m_currentHealthThreshold;
    }
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

void VineAI::pursueTarget(void)
{
    PursueTarget( );
}

void VineAI::goHome(void)
{
    GoToHomeLocation( );
}

#endif
