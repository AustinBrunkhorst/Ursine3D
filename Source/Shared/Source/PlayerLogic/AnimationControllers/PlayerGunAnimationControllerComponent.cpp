/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerGunAnimationControllerComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "PlayerGunAnimationControllerComponent.h"

#include "GameEvents.h"
#include "PlayEntityAnimationClipState.h"
#include "GunReloadFinishedState.h"
#include "UIScreensConfigComponent.h"
#include "PlayerIdComponent.h"
#include "HitscanWeaponComponent.h"

#include <TriggerCondition.h>
#include <BoolCondition.h>

NATIVE_COMPONENT_DEFINITION(PlayerGunAnimationController);

using namespace ursine;
using namespace ecs;

namespace
{
    const std::string kJump = "Jump";
    const std::string kMoving = "Moving";
    const std::string kShooting = "Shooting";
    const std::string kReload = "Reload";
}

PlayerGunAnimationController::PlayerGunAnimationController(void)
    : BaseComponent( )
    , m_connected( false )
    , m_reloadTime( 1.0f ) { }

PlayerGunAnimationController::~PlayerGunAnimationController(void)
{
    if (m_connected)
        connectToEvents( false );
}

void PlayerGunAnimationController::OnSceneReady(Scene *scene)
{
    auto animator = GetOwner( )->GetComponent<Animator>( );

    m_controller = std::make_shared<AnimatorControllerStateMachine>(
        "Player Gun Animation Controller", animator
    );

    // Create the states
    auto idle = m_controller->AddState<PlayAnimationState>( m_idleState );
    auto run = m_controller->AddState<PlayAnimationState>( m_runState );
    auto jump = m_controller->AddState<PlayAnimationState>( m_jumpState );
    auto shootIn = m_controller->AddState<PlayAnimationState>( m_shootBeginState, true );
    auto shootOut = m_controller->AddState<PlayAnimationState>( m_shootEndState, false );
    auto shootLoop = m_controller->AddState<PlayAnimationState>( m_shootLoopState, false, 1.5f );
    auto reload = m_controller->AddState<PlayEntityAnimationClipState>( "Reload", true );
    auto messageUI = m_controller->AddState<GunReloadFinishedState>( );

    // Add the variables
    m_controller->AddTrigger( kJump );
    m_controller->AddBool( kMoving, false );
    m_controller->AddBool( kShooting, false );
    m_controller->AddBool( kReload, false );

    // Add the transitions
    idle->AddTransition( jump, "Jumping" )
        ->AddCondition<sm::TriggerCondition>( kJump );

    jump->AddTransition( idle, "Back To Idle" );

    idle->AddTransition( run, "Running" )
        ->AddCondition<sm::BoolCondition>( kMoving, true );

    run->AddTransition( idle, "Back To Idle" )
        ->AddCondition<sm::BoolCondition>( kMoving, false );

    idle->AddTransition( shootIn, "To Shoot" )
        ->AddCondition<sm::BoolCondition>( kShooting, true );
    run->AddTransition( shootIn, "To Shoot" )
       ->AddCondition<sm::BoolCondition>( kShooting, true );
    jump->AddTransition( shootIn, "To Shoot" )
        ->AddCondition<sm::BoolCondition>( kShooting, true );

    shootIn->AddTransition( shootLoop, "To Shoot Loop" )
           ->AddCondition<sm::BoolCondition>( kReload, false );
    shootLoop->AddTransition( shootOut, "To Shoot Out" )
             ->AddCondition<sm::BoolCondition>( kShooting, false );
    shootOut->AddTransition( idle, "To Idle" )
            ->AddCondition<sm::BoolCondition>( kMoving, false );
    shootOut->AddTransition( run, "To Run" )
            ->AddCondition<sm::BoolCondition>( kMoving, true );

    reload->AddTransition( messageUI, "Message UI" );

    idle->AddTransition( reload, "reload" )->AddCondition<sm::BoolCondition>( kReload, true );
    run->AddTransition( reload, "reload" )->AddCondition<sm::BoolCondition>( kReload, true );
    jump->AddTransition( reload, "reload" )->AddCondition<sm::BoolCondition>( kReload, true );
    shootLoop->AddTransition( shootOut, "reload" )->AddCondition<sm::BoolCondition>( kReload, true );
    shootOut->AddTransition( reload, "reload" )->AddCondition<sm::BoolCondition>( kReload, true );

    {
        auto trans = messageUI->AddTransition( idle, "To Idle" );
        trans->AddCondition<sm::BoolCondition>( kMoving, false );
        trans->AddCondition<sm::BoolCondition>( kReload, false );
    }

    {
        auto trans = messageUI->AddTransition( run, "To Idle" );
        trans->AddCondition<sm::BoolCondition>( kMoving, true );
        trans->AddCondition<sm::BoolCondition>( kReload, false );
    }

    // Set the initial state
    m_controller->SetInitialState( idle );

    // Subscribe to update and hierarchy serialized
    GetOwner( )->GetWorld( )->Listener( this )
        .On( WORLD_UPDATE, &PlayerGunAnimationController::onUpdate );
}

const std::string &PlayerGunAnimationController::GetIdleState(void) const
{
    return m_idleState;
}

void PlayerGunAnimationController::SetIdleState(const std::string &state)
{
    m_idleState = state;

    NOTIFY_COMPONENT_CHANGED( "idleState", m_idleState );
}

const std::string &PlayerGunAnimationController::GetJumpState(void) const
{
    return m_jumpState;
}

void PlayerGunAnimationController::SetJumpState(const std::string &state)
{
    m_jumpState = state;

    NOTIFY_COMPONENT_CHANGED( "jumpState", m_jumpState );
}

const std::string &PlayerGunAnimationController::GetRunState(void) const
{
    return m_runState;
}

void PlayerGunAnimationController::SetRunState(const std::string &state)
{
    m_runState = state;

    NOTIFY_COMPONENT_CHANGED( "runState", m_runState );
}

const std::string &PlayerGunAnimationController::GetShootBeginState(void) const
{
    return m_shootBeginState;
}

void PlayerGunAnimationController::SetShootBeginState(const std::string &state)
{
    m_shootBeginState = state;

    NOTIFY_COMPONENT_CHANGED( "shootBeginState", m_shootBeginState );
}

const std::string &PlayerGunAnimationController::GetShootLoopState(void) const
{
    return m_shootLoopState;
}

void PlayerGunAnimationController::SetShootLoopState(const std::string &state)
{
    m_shootLoopState = state;

    NOTIFY_COMPONENT_CHANGED( "shootLoopState", m_shootLoopState );
}

const std::string &PlayerGunAnimationController::GetShootEndState(void) const
{
    return m_shootEndState;
}

void PlayerGunAnimationController::SetShootEndState(const std::string &state)
{
    m_shootEndState = state;

    NOTIFY_COMPONENT_CHANGED( "shootEndState", m_shootEndState );
}

const std::string &PlayerGunAnimationController::GetReloadClip(void) const
{
    return m_reloadClip;
}

void PlayerGunAnimationController::SetReloadClip(const std::string &clip)
{
    m_reloadClip = clip;

    NOTIFY_COMPONENT_CHANGED( "reloadClip", m_reloadClip );
}

void PlayerGunAnimationController::onUpdate(EVENT_HANDLER(World))
{
    if (!m_connected)
        connectToEvents( true );

    m_controller->Update( );

    // Reset the moving bool
    m_controller->SetBool( kMoving, false );
}

void PlayerGunAnimationController::connectToEvents(bool toggle)
{
    if (toggle)
    {
        GetOwner( )->GetRoot( )->Listener( this )
            .On( game::JUMP_COMMAND, &PlayerGunAnimationController::onJump )
            .On( game::MOVEMENT_COMMAND, &PlayerGunAnimationController::onMove )
            .On( game::FIRE_START, &PlayerGunAnimationController::onStartShoot )
            .On( game::FIRE_END, &PlayerGunAnimationController::onEndShoot )
            .On( game::RELOAD_START, &PlayerGunAnimationController::onStartReload )
            .On( game::RELOAD_END, &PlayerGunAnimationController::onEndReload )
            .On( game::GUN_SHOOT, &PlayerGunAnimationController::onShoot );

        m_connected = true;
    }
    else
    {
        GetOwner( )->GetRoot( )->Listener( this )
            .Off( game::JUMP_COMMAND, &PlayerGunAnimationController::onJump )
            .Off( game::MOVEMENT_COMMAND, &PlayerGunAnimationController::onMove )
            .Off( game::FIRE_START, &PlayerGunAnimationController::onStartShoot )
            .Off( game::FIRE_END, &PlayerGunAnimationController::onEndShoot )
            .Off( game::RELOAD_START, &PlayerGunAnimationController::onStartReload )
            .Off( game::RELOAD_END, &PlayerGunAnimationController::onEndReload )
            .Off( game::GUN_SHOOT, &PlayerGunAnimationController::onShoot );

        m_connected = false;
    }
}

void PlayerGunAnimationController::onJump(EVENT_HANDLER(Entity))
{
    m_controller->SetTrigger( kJump );
}

void PlayerGunAnimationController::onMove(EVENT_HANDLER(Entity))
{
    m_controller->SetBool( kMoving, true );
}

void PlayerGunAnimationController::onStartShoot(EVENT_HANDLER(Entity))
{
    if (!m_controller->GetBool( kReload ))
        m_controller->SetBool( kShooting, true );
}

void PlayerGunAnimationController::onEndShoot(EVENT_HANDLER(Entity))
{
    m_controller->SetBool( kShooting, false );
}

void PlayerGunAnimationController::onStartReload(EVENT_HANDLER(Entity))
{
    m_controller->SetBool( kReload, true );
}

void PlayerGunAnimationController::onEndReload(EVENT_HANDLER(Entity))
{
    m_controller->SetBool( kReload, false );
}

void PlayerGunAnimationController::onShoot(EVENT_HANDLER(Entity))
{
    EVENT_SENDER(Entity, sender);

    // message the UI that the gun is reloaded
    auto world = sender->GetWorld( );
    auto *scene = world->GetOwner( );

    UAssert( scene != nullptr,
        "Scene was null."    
    );

    auto *ui = world->GetSettings( )->GetComponent<UIScreensConfig>( );

    UAssert( ui != nullptr,
        "UIConfig was null."
    );

    auto root = sender->GetRoot( );
    auto player = root->GetComponent<PlayerID>( );
    auto gun = root->GetComponentInChildren<HitscanWeapon>( );

    UAssert( gun, "Error: There should be a gun son" );

    ui_event::PlayerAmmoUpdated e;

    e.percent = static_cast<float>( gun->GetClipCount( ) ) / 
                gun->GetClipSize( );
    e.playerID = player->GetID( );

    ui->TriggerPlayerHUDEvent( e );
}
