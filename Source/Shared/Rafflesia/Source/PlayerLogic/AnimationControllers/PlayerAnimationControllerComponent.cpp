/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerAnimationControllerComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "PlayerAnimationControllerComponent.h"

#include "GameEvents.h"

#include <BoolCondition.h>
#include <AudioEmitterComponent.h>

NATIVE_COMPONENT_DEFINITION(PlayerAnimationController);

using namespace ursine;
using namespace ecs;
using namespace resources;

namespace
{
    const std::string kRun = "Run";
    const std::string kRunRight = "RunRight";
    const std::string kRunLeft = "RunLeft";

    const std::string kJump = "Jump";
    const std::string kAirborn = "Airborn";

    const std::string kWon = "Won";

    const std::string kDead = "Dead";
}

PlayerAnimationController::PlayerAnimationController(void)
    : BaseComponent( )
    , m_connected( false )
    , m_sweptController( nullptr )
    , m_characterController( nullptr ) { }

PlayerAnimationController::~PlayerAnimationController(void)
{
    if (m_connected)
        connectToEvents( false );
}

void PlayerAnimationController::OnSceneReady(Scene *scene)
{
    auto animator = GetOwner( )->GetComponent<Animator>( );

    m_controller = std::make_shared<AnimatorControllerStateMachine>(
        "Player Animation Controller", animator
    );

    // create the states
    auto idle = m_controller->AddState<PlayAnimationState>( m_idleState );
    auto run = m_controller->AddState<PlayAnimationState>( m_runState, false, 2.0f );
    auto runR = m_controller->AddState<PlayAnimationState>( m_runRightState, false, 2.0f );
    auto runL = m_controller->AddState<PlayAnimationState>( m_runLeftState, false, 2.0f );
    auto jumpIn = m_controller->AddState<PlayAnimationState>( m_jumpInState );
    auto jumpLoop = m_controller->AddState<PlayAnimationState>( m_jumpLoopState );
    auto jumpOut = m_controller->AddState<PlayAnimationState>( m_jumpOutState );
    auto die = m_controller->AddState<PlayAnimationState>( m_dieState, false, 1.0f, true );
    auto win = m_controller->AddState<PlayAnimationState>( m_winState, false, 1.0f, true );

    // Add the variables
    m_controller->AddBool( kRun, false );
    m_controller->AddBool( kRunRight, false );
    m_controller->AddBool( kRunLeft, false );
    m_controller->AddBool( kJump, false );
    m_controller->AddBool( kAirborn, false );
    m_controller->AddBool( kWon, false );
    m_controller->AddBool( kDead, false );

    // Setup transitions
    idle->AddTransition( jumpIn, "Jump" )->AddCondition<sm::BoolCondition>( kJump, true );
    idle->AddTransition( jumpLoop, "Jump Cycle" )->AddCondition<sm::BoolCondition>( kAirborn, true );
    jumpIn->AddTransition( jumpLoop, "Jump Cycle" )->AddCondition<sm::BoolCondition>( kAirborn, true );
    jumpLoop->AddTransition( jumpOut, "Jump Out" )->AddCondition<sm::BoolCondition>( kAirborn, false );
    jumpOut->AddTransition( idle, "Idle" )->AddCondition<sm::BoolCondition>( kAirborn, false );
    jumpIn->AddTransition( jumpOut, "Jump Out" )->AddCondition<sm::BoolCondition>( kAirborn, false );

    idle->AddTransition( die, "Die" )->AddCondition<sm::BoolCondition>( kDead, true );
    die->AddTransition( idle, "Idle" )->AddCondition<sm::BoolCondition>( kDead, false );

    idle->AddTransition( win, "Win" )->AddCondition<sm::BoolCondition>( kWon, true );
    win->AddTransition( idle, "Win" )->AddCondition<sm::BoolCondition>( kWon, false );

    idle->AddTransition( run, "Run" )->AddCondition<sm::BoolCondition>( kRun, true );
    idle->AddTransition( runR, "RunR" )->AddCondition<sm::BoolCondition>( kRunRight, true );
    idle->AddTransition( runL, "RunL" )->AddCondition<sm::BoolCondition>( kRunLeft, true );

    run->AddTransition( die, "Die" )->AddCondition<sm::BoolCondition>( kDead, true );
    runR->AddTransition( die, "Die" )->AddCondition<sm::BoolCondition>( kDead, true );
    runL->AddTransition( die, "Die" )->AddCondition<sm::BoolCondition>( kDead, true );

    run->AddTransition( win, "Win" )->AddCondition<sm::BoolCondition>( kWon, true );
    runR->AddTransition( win, "Win" )->AddCondition<sm::BoolCondition>( kWon, true );
    runL->AddTransition( win, "Win" )->AddCondition<sm::BoolCondition>( kWon, true );

    run->AddTransition( jumpIn, "Jump" )->AddCondition<sm::BoolCondition>( kJump, true );
    runR->AddTransition( jumpIn, "Jump" )->AddCondition<sm::BoolCondition>( kJump, true );
    runL->AddTransition( jumpIn, "Jump" )->AddCondition<sm::BoolCondition>( kJump, true );

    run->AddTransition( jumpLoop, "Airborn" )->AddCondition<sm::BoolCondition>( kAirborn, true );
    runR->AddTransition( jumpLoop, "Airborn" )->AddCondition<sm::BoolCondition>( kAirborn, true );
    runL->AddTransition( jumpLoop, "Airborn" )->AddCondition<sm::BoolCondition>( kAirborn, true );

    // Run to idle
    {
        auto trans = run->AddTransition( idle, "Idle" );

        // !Run && !RunLeft && !RunRight && !Jump && !Airborn && !Win && !Dead
        trans->AddCondition<sm::BoolCondition>( kRun, false );
        trans->AddCondition<sm::BoolCondition>( kRunRight, false );
        trans->AddCondition<sm::BoolCondition>( kRunLeft, false );
        trans->AddCondition<sm::BoolCondition>( kJump, false );
        trans->AddCondition<sm::BoolCondition>( kAirborn, false );
        trans->AddCondition<sm::BoolCondition>( kWon, false );
        trans->AddCondition<sm::BoolCondition>( kDead, false );
    }

    // Run Right to idle
    {
        auto trans = runR->AddTransition( idle, "Idle" );

        // !Run && !RunLeft && !RunRight && !Jump && !Airborn && !Win && !Dead
        trans->AddCondition<sm::BoolCondition>( kRun, false );
        trans->AddCondition<sm::BoolCondition>( kRunRight, false );
        trans->AddCondition<sm::BoolCondition>( kRunLeft, false );
        trans->AddCondition<sm::BoolCondition>( kJump, false );
        trans->AddCondition<sm::BoolCondition>( kAirborn, false );
        trans->AddCondition<sm::BoolCondition>( kWon, false );
        trans->AddCondition<sm::BoolCondition>( kDead, false );
    }

    // Run Left to idle
    {
        auto trans = runL->AddTransition( idle, "Idle" );

        // !Run && !RunLeft && !RunRight && !Jump && !Airborn && !Win && !Dead
        trans->AddCondition<sm::BoolCondition>( kRun, false );
        trans->AddCondition<sm::BoolCondition>( kRunRight, false );
        trans->AddCondition<sm::BoolCondition>( kRunLeft, false );
        trans->AddCondition<sm::BoolCondition>( kJump, false );
        trans->AddCondition<sm::BoolCondition>( kAirborn, false );
        trans->AddCondition<sm::BoolCondition>( kWon, false );
        trans->AddCondition<sm::BoolCondition>( kDead, false );
    }

    // Run to run left
    {
        // RunL && !Jump && !Airborn && !Win && !Dead
        auto trans = run->AddTransition( runL, "Run Left" );
        
        trans->AddCondition<sm::BoolCondition>( kRunLeft, true );
        trans->AddCondition<sm::BoolCondition>( kJump, false );
        trans->AddCondition<sm::BoolCondition>( kAirborn, false );
        trans->AddCondition<sm::BoolCondition>( kWon, false );
        trans->AddCondition<sm::BoolCondition>( kDead, false );
    }

    // Run to run right
    {
        // RunR && !Jump && !Airborn && !Win && !Dead
        auto trans = run->AddTransition( runR, "Run Right" );
        
        trans->AddCondition<sm::BoolCondition>( kRunRight, true );
        trans->AddCondition<sm::BoolCondition>( kJump, false );
        trans->AddCondition<sm::BoolCondition>( kAirborn, false );
        trans->AddCondition<sm::BoolCondition>( kWon, false );
        trans->AddCondition<sm::BoolCondition>( kDead, false );
    }

    // Run Right to Run
    {
        // Run && !Jump && !Airborn && !Win && !Dead
        auto trans = runR->AddTransition( run, "Run" );
        
        trans->AddCondition<sm::BoolCondition>( kRun, true );
        trans->AddCondition<sm::BoolCondition>( kJump, false );
        trans->AddCondition<sm::BoolCondition>( kAirborn, false );
        trans->AddCondition<sm::BoolCondition>( kWon, false );
        trans->AddCondition<sm::BoolCondition>( kDead, false );
    }

    // Run Left to Run
    {
        // Run && !Jump && !Airborn && !Win && !Dead
        auto trans = runL->AddTransition( run, "Run" );
        
        trans->AddCondition<sm::BoolCondition>( kRun, true );
        trans->AddCondition<sm::BoolCondition>( kJump, false );
        trans->AddCondition<sm::BoolCondition>( kAirborn, false );
        trans->AddCondition<sm::BoolCondition>( kWon, false );
        trans->AddCondition<sm::BoolCondition>( kDead, false );
    }

    // Run Left to Run Right
    {
        // RunR && !Jump && !Airborn && !Win && !Dead
        auto trans = runL->AddTransition( runR, "RunR" );
        
        trans->AddCondition<sm::BoolCondition>( kRunRight, true );
        trans->AddCondition<sm::BoolCondition>( kJump, false );
        trans->AddCondition<sm::BoolCondition>( kAirborn, false );
        trans->AddCondition<sm::BoolCondition>( kWon, false );
        trans->AddCondition<sm::BoolCondition>( kDead, false );
    }

    // Run Right to Run Left
    {
        // RunR && !Jump && !Airborn && !Win && !Dead
        auto trans = runR->AddTransition( runL, "RunL" );
        
        trans->AddCondition<sm::BoolCondition>( kRunLeft, true );
        trans->AddCondition<sm::BoolCondition>( kJump, false );
        trans->AddCondition<sm::BoolCondition>( kAirborn, false );
        trans->AddCondition<sm::BoolCondition>( kWon, false );
        trans->AddCondition<sm::BoolCondition>( kDead, false );
    }

    // Set the initial state
    m_controller->SetInitialState( idle );

    // Subscribe to update the hierarchy serialized
    GetOwner( )->GetWorld( )->Listener( this )
        .On( WORLD_UPDATE, &PlayerAnimationController::onUpdate );
}

const std::string &PlayerAnimationController::GetIdleState(void) const
{
    return m_idleState;
}

void PlayerAnimationController::SetIdleState(const std::string &state)
{
    m_idleState = state;

    NOTIFY_COMPONENT_CHANGED( "idleState", m_idleState );
}

const std::string &PlayerAnimationController::GetRunState(void) const
{
    return m_runState;
}

void PlayerAnimationController::SetRunState(const std::string &state)
{
    m_runState = state;

    NOTIFY_COMPONENT_CHANGED( "runState", m_runState );
}

const std::string &PlayerAnimationController::GetRunLeftState(void) const
{
    return m_runLeftState;
}

void PlayerAnimationController::SetRunLeftState(const std::string &state)
{
    m_runLeftState = state;

    NOTIFY_COMPONENT_CHANGED( "runLeftState", m_runLeftState );
}

const std::string &PlayerAnimationController::GetRunRightState(void) const
{
    return m_runRightState;
}

void PlayerAnimationController::SetRunRightState(const std::string &state)
{
    m_runRightState = state;

    NOTIFY_COMPONENT_CHANGED( "runRightState", m_runRightState );
}

const std::string &PlayerAnimationController::GetJumpInState(void) const
{
    return m_jumpInState;
}

void PlayerAnimationController::SetJumpInState(const std::string &state)
{
    m_jumpInState = state;

    NOTIFY_COMPONENT_CHANGED( "jumpInState", m_jumpInState );
}

const std::string &PlayerAnimationController::GetJumpLoopState(void) const
{
    return m_jumpLoopState;
}

void PlayerAnimationController::SetJumpLoopState(const std::string &state)
{
    m_jumpLoopState = state;

    NOTIFY_COMPONENT_CHANGED( "jumpLoopState", m_jumpLoopState );
}

const std::string &PlayerAnimationController::GetJumpOutState(void) const
{
    return m_jumpOutState;
}

void PlayerAnimationController::SetJumpOutState(const std::string &state)
{
    m_jumpOutState = state;

    NOTIFY_COMPONENT_CHANGED( "jumpOutState", m_jumpOutState );
}

const std::string &PlayerAnimationController::GetDieState(void) const
{
    return m_dieState;
}

void PlayerAnimationController::SetDieState(const std::string &state)
{
    m_dieState = state;

    NOTIFY_COMPONENT_CHANGED( "dieState", m_dieState );
}

const std::string &PlayerAnimationController::GetWinState(void) const
{
    return m_winState;
}

const ResourceReference &PlayerAnimationController::GetLaughSfx(void)
{
    return m_laughSfx;
}

void PlayerAnimationController::SetLaughSfx(const ResourceReference &sfx)
{
    m_laughSfx = sfx;

    NOTIFY_COMPONENT_CHANGED( "laughSfx", m_laughSfx );
}

void PlayerAnimationController::SetWinState(const std::string &state)
{
    m_winState = state;

    NOTIFY_COMPONENT_CHANGED( "winState", m_winState );
}

void PlayerAnimationController::SetWonFlag(bool flag)
{
    m_controller->SetBool( kWon, flag );

    auto emitter = GetOwner( )->GetRoot( )->GetComponent<AudioEmitter>( );

    if (flag)
    {
        GetOwner( )->GetTimers( ).Create( TimeSpan::FromSeconds( math::Rand( 0.5f, 2.0f ) ) )
            .Repeat( math::Rand( 1, 4 ) )
            .Repeated( [=] {
                emitter->PushEvent( m_laughSfx );
            } );
    }
}

void PlayerAnimationController::onUpdate(EVENT_HANDLER(World))
{
    if (!m_connected)
        connectToEvents( true );

    m_controller->SetBool( kAirborn, !m_sweptController->GetGrounded( ) );

    m_controller->Update( );

    // reset the flags
    m_controller->SetBool( kRun, false );
    m_controller->SetBool( kRunRight, false );
    m_controller->SetBool( kRunLeft, false );
    m_controller->SetBool( kJump, false );
}

void PlayerAnimationController::connectToEvents(bool toggle)
{
    auto root = GetOwner( )->GetRoot( );

    if (toggle)
    {
        root->Listener( this )
            .On( game::JUMP_COMMAND, &PlayerAnimationController::onJump )
            .On( game::MOVEMENT_COMMAND, &PlayerAnimationController::onMove )
            .On( game::REVIVE_PLAYER, &PlayerAnimationController::onRevive );

        root->GetComponent<Health>( )->Listener( this )
            .On( HEALTH_ZERO, &PlayerAnimationController::onDeath );

        m_sweptController = root->GetComponent<SweptController>( );
        m_characterController = root->GetComponent<CharacterController>( );
    }
    else
    {
        root->Listener( this )
            .Off( game::JUMP_COMMAND, &PlayerAnimationController::onJump )
            .Off( game::MOVEMENT_COMMAND, &PlayerAnimationController::onMove )
            .Off( game::REVIVE_PLAYER, &PlayerAnimationController::onRevive );

        root->GetComponent<Health>( )->Listener( this )
            .Off( HEALTH_ZERO, &PlayerAnimationController::onDeath );
    }
}

void PlayerAnimationController::onJump(EVENT_HANDLER(ursine::ecs::Entity))
{
    if (m_sweptController->GetGrounded( ) && !m_characterController->GetLockMovement( ))
        m_controller->SetBool( kJump, true );
}

void PlayerAnimationController::onMove(EVENT_HANDLER(ursine::ecs::Entity))
{
    if (!m_sweptController->GetGrounded( ) || m_characterController->GetLockMovement( ))
        return;

    EVENT_ATTRS(Entity, game::MovementEventArgs);

    auto &dir = args->m_moveDir;

    if (dir.Length( ) <= math::Epsilon)
        return;

    if (abs( dir.X( ) ) >= abs( dir.Y( ) ))
    {
        if (dir.X( ) < 0.0f)
            m_controller->SetBool( kRunLeft, true );
        else
            m_controller->SetBool( kRunRight, true );
    }
    else
    {
        m_controller->SetBool( kRun, true );
    }
}

void PlayerAnimationController::onDeath(EVENT_HANDLER(Health))
{
    m_controller->SetBool( kDead, true );
}

void PlayerAnimationController::onRevive(EVENT_HANDLER(Entity))
{
    m_controller->SetBool( kDead, false );\
}
