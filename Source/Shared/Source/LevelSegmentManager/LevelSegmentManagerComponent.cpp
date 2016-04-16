/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** LevelManagerSystem.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "LevelSegmentManagerComponent.h"

#include "InitializeSegmentState.h"
#include "SpawnPlayersState.h"
#include "PlayerViewportTweeningState.h"
#include "LockPlayerCharacterControllerState.h"
#include "ChangeSegmentState.h"
#include "CombatBowl1IntroCinematicState.h"
#include "RepositionPlayersAndCloseDoorState.h"
#include "BossRoomTopAnimationState.h"
#include "ToggleLightGroupState.h"
#include "TriggerWaitState.h"
#include "ToggleHudState.h"
#include "Phase3WaitForTriggerState.h"
#include "ToggleCameraActiveState.h"
#include "PlayEntityAnimatorState.h"
#include "TutorialWaitForPlayerReviveState.h"
#include "TutorialVineHandlerState.h"
#include "PlayerWinState.h"
#include "ScreenFadeState.h"

#include "CurrentSegmentCondition.h"

#include "TutorialResourcesComponent.h"
#include "CombatBowl1ResourcesComponent.h"
#include "BossRoomResourcesComponent.h"
#include "EndingCreditsResourcesComponent.h"

#include "UIScreensConfigComponent.h"

#include <Scene.h>
#include <TimerCondition.h>

#include <iostream>

#if defined(URSINE_WITH_EDITOR)

#include <Notification.h>

#endif

NATIVE_COMPONENT_DEFINITION( LevelSegmentManager );

using namespace ursine;
using namespace ecs;

LevelSegmentManager::LevelSegmentManager(void)
    : BaseComponent( )
    , EventDispatcher( this )
    , m_segment( LevelSegments::Empty )
    , m_player1( )
    , m_player2( )
    , m_enableDebugOutput( false ) { }

LevelSegmentManager::~LevelSegmentManager(void)
{
    GetOwner( )->GetWorld( )->Listener( this )
        .Off( WORLD_UPDATE, &LevelSegmentManager::onUpdate );
}

LevelSegments LevelSegmentManager::GetCurrentSegment(void) const
{
    return m_segment;
}

void LevelSegmentManager::SetCurrentSegment(LevelSegments segment)
{
    if (segment == m_segment)
        return;

    m_segment = segment;

    LevelSegmentChangeArgs args;

    args.segment = segment;

    Dispatch( LevelSegmentManagerEvents::SegmentChanged, &args );

    NOTIFY_COMPONENT_CHANGED( "currentSegment", m_segment );
}

bool LevelSegmentManager::GetEnableDebugOutput(void) const
{
    return m_enableDebugOutput;
}

void LevelSegmentManager::SetEnableDebugOutput(bool enable)
{
    m_enableDebugOutput = enable;
}

const EntityHandle &LevelSegmentManager::GetPlayer1(void)
{
    return m_player1;
}

const EntityHandle &LevelSegmentManager::GetPlayer2(void)
{
    return m_player2;
}

void LevelSegmentManager::OnSceneReady(Scene *scene)
{
    auto *world = GetOwner( )->GetWorld( );

    // subscribe to update
    world->Listener( this )
        .On( WORLD_UPDATE, &LevelSegmentManager::onUpdate );

    // add and initialize all segment logic
    initTutorialLogic( );

    initCombatBowl1Logic( );
    initConduitTutLogic( );
    
    initCombatBowl2Logic( );
    initEmpowerDisempowerTutLogic( );

    initCombatBowl3Logic( );
    initAccumulateTutLogic( );

    initCombatBowl4Logic( );
    initBossRoomLogic( );
}

void LevelSegmentManager::initTutorialLogic(void)
{
    auto resources = GetOwner( )->GetComponent<TutorialResources>( );

    // Create a state machine that initializes the scene
    auto stateM = std::make_shared<SegmentLogicStateMachine>( "Init Tutorial", this );

    // Initial state for spawning the level
    auto initState = stateM->AddState<InitializeSegmentState>(
        resources->GetWorldData( ),
        LevelSegments::BossRoom_Platforming
    );

    auto playerCreateState = stateM->AddState<SpawnPlayersState>( true, true );
    auto lockCCState = stateM->AddState<LockPlayerCharacterControllerState>( true, true, true, true );
    auto tweenState = stateM->AddState<PlayerViewportTweeningState>( ViewportTweenType::SplitInUpDown, true, true );
    auto unlockCCState = stateM->AddState<LockPlayerCharacterControllerState>( false, false, false, false );
    auto hudOn = stateM->AddState<ToggleHudState>( true );
    auto changeSegState = stateM->AddState<ChangeSegmentState>( LevelSegments::Tut_GateOpensTutorial );
    auto waitForRevive = stateM->AddState<TutorialWaitForPlayerReviveState>( );
    auto vineHandler = stateM->AddState<TutorialVineHandlerState>( resources->GetVineArchetype( ) );

    // Next state for spawning the players
    initState->AddTransition( playerCreateState, "Go To Init Players" );

    // Make sure the players have their character controller's locked
    playerCreateState->AddTransition( lockCCState, "Go To Locking Player Controller" );

    // After players are spawned tween their viewports
    auto introCin = lockCCState->AddTransition( tweenState, "Go To Tween Viewports" );

    introCin->AddCondition<sm::TimerCondition>(TimeSpan::FromSeconds(28.0f));

    // Unlock players
    tweenState->AddTransition( unlockCCState, "Go To Unlocking Player Controller" );

    unlockCCState->AddTransition( hudOn, "Turn Hud On" );

    // Halt the state machine until the players hit the first trigger, so that we can then
    // Spawn the movmement tutorial prompts
    //auto waitForTrigger = stateM->AddState<TriggerWaitState>( resources->GetGateTriggerName( ) );

    // After the viewports tween out change the level segment
    hudOn->AddTransition( changeSegState, "To Gate Opens" );

    changeSegState->AddTransition( waitForRevive, "Revive Dat Ass" );

    waitForRevive->AddTransition( vineHandler, "To Vine Handler" )
                 ->AddCondition<sm::CurrentSegmentCondition>( this, LevelSegments::Tut_SpawnVinesTutorial );
    
    stateM->SetInitialState( initState );

    addSegmentLogic( stateM, {
        LevelSegments::Tut_OpeningCinematic, // Introduce players?  What about dome
        LevelSegments::Tut_GateOpensTutorial,
        LevelSegments::Tut_MovementTutorial, // Tell the player to move to a position
        LevelSegments::Tut_JumpTutorial, // Tell the player to jump
        LevelSegments::Tut_HipFireTutorial, // Shooting targets
        LevelSegments::Tut_AimFireTutorial, // Shooting targets
        LevelSegments::Tut_ShootMovingTargetsTutorial, // Shooting moving targets
        LevelSegments::Tut_HallwayInterumTutorial, // The player walks to the last zone
        LevelSegments::Tut_KillPlayerTutorial, // Kill one player
        LevelSegments::Tut_ReviveTutorial, // Revive mechanic (kill one player, have other revive the other)
        LevelSegments::Tut_SpawnVinesTutorial, // Must Defeat Vines to exit
        LevelSegments::Tut_DoorOpenTutorial // Vines defeated
    } );

    // This state machine is used for fading to white, then chaing segments to the first boss segment
    auto sm = std::make_shared<SegmentLogicStateMachine>( "Fade to white, then to boss", this );

    auto fade = sm->AddState<ScreenFadeState>( 2.0f, 3.0f, 2.0f, Color::White );
    auto changeSeg = sm->AddState<ChangeSegmentState>( LevelSegments::BossRoom_Platforming );

    fade->AddTransition( changeSeg, "Change segment when fading is done" )
        ->AddCondition<sm::TimerCondition>( TimeSpan::FromSeconds( 2.1f ) );

    sm->SetInitialState( fade );

    addSegmentLogic( sm, { LevelSegments::Tut_SimulationCreationCinematic } );
}

void LevelSegmentManager::initCombatBowl1Logic(void)
{
    auto resources = GetOwner( )->GetComponent<CombatBowl1Resources>( );

    if (!resources)
        return;

    // Create a state machine that initializes the scene
    auto stateM = std::make_shared<SegmentLogicStateMachine>( "Init Combat Bowl", this );

    // Initial state for spawning the level
    auto initState = stateM->AddState<InitializeSegmentState>(
        resources->GetWorldData( ),
        LevelSegments::Empty
    );

    // Next state for spawning the players (reposition them if they are present)
    auto playerCreateState = stateM->AddState<SpawnPlayersState>( true, true );

    auto toAddPlayers = initState->AddTransition( playerCreateState, "Go To Init Players" );

    toAddPlayers->AddCondition<sm::CurrentSegmentCondition>( 
        this, LevelSegments::CB1_SimulationStartCinematic, LevelSegments::CB1_OpenConduitRoom
    );

    // Lock the players
    auto lockPlayers = stateM->AddState<LockPlayerCharacterControllerState>( true, true, true, true );

    playerCreateState->AddTransition( lockPlayers, "To Lock Players" );

    // Then start the cinematic
    auto cinematicState = stateM->AddState<CombatBowl1IntroCinematicState>( );

    lockPlayers->AddTransition( cinematicState, "Go To Start Cinematic" );

    // After the cinematic is finshed (it's a blocking state), tween the viewports and unlock the players
    auto viewportTween = stateM->AddState<PlayerViewportTweeningState>( ViewportTweenType::SplitInUpDown, true, true );

    cinematicState->AddTransition( viewportTween, "Go To Tween Viewports" );

    // Then once they are tweened, unlock the players
    auto unlockPlayers = stateM->AddState<LockPlayerCharacterControllerState>( false, false, false, false );

    viewportTween->AddTransition( unlockPlayers, "Go To Unlock Player's Controllers" );

    // Then change the segment to start the spawning
    auto changeSegment = stateM->AddState<ChangeSegmentState>( LevelSegments::CB1_Combat1 );

    unlockPlayers->AddTransition( changeSegment, "Go To Change Segment To Combat 1" );

    stateM->SetInitialState( initState );

    addSegmentLogic(stateM, {
        LevelSegments::CB1_SimulationStartCinematic,
        LevelSegments::CB1_WeaponSelection,
        LevelSegments::CB1_ActivateSystems1,
        LevelSegments::CB1_Combat1,
        LevelSegments::CB1_Combat2,
        LevelSegments::CB1_Combat3,
        LevelSegments::CB1_Combat4,
        LevelSegments::CB1_OpenConduitRoom,
        LevelSegments::CB4_OpenBossRoom,
        LevelSegments::BossRoom_Introduction,
        LevelSegments::BossRoom_Phase1
    } );
}

void LevelSegmentManager::initConduitTutLogic(void)
{
}

void LevelSegmentManager::initCombatBowl2Logic(void)
{
}

void LevelSegmentManager::initEmpowerDisempowerTutLogic(void)
{
}

void LevelSegmentManager::initCombatBowl3Logic(void)
{
}

void LevelSegmentManager::initAccumulateTutLogic(void)
{
}

void LevelSegmentManager::initCombatBowl4Logic(void)
{
}

void LevelSegmentManager::initBossRoomLogic(void)
{
    auto resources = GetOwner( )->GetComponent<BossRoomResources>( );

    // Spawn all boss room archetypes
    {
        auto initStateM = std::make_shared<SegmentLogicStateMachine>( "Init Boss Room", this );

        // initial state for spawning the level
        auto initState = initStateM->AddState<InitializeSegmentState>(
            resources->GetWorldData( ),
            LevelSegments::EndingCredits
        );

        // Next state for spawning the players (reposition them if they are present)
        auto playerCreateState = initStateM->AddState<SpawnPlayersState>( true, false );

        initState->AddTransition( playerCreateState, "Go To Init Players" );

        initStateM->SetInitialState( initState );

        addSegmentLogic( initStateM, {
            LevelSegments::CB4_OpenBossRoom,
            LevelSegments::BossRoom_Platforming,
            LevelSegments::BossRoom_Introduction,
            LevelSegments::BossRoom_Phase1,
            LevelSegments::BossRoom_Phase2,
            LevelSegments::BossRoom_Phase3,
            LevelSegments::BossRoom_Phase4,
            LevelSegments::BossRoom_Phase5
        } );
    }

    // Turn phase 1 & 2 lights on
    {
        auto sm = std::make_shared<SegmentLogicStateMachine>( "Phse 1 & 2 Lights On", this );
        sm->SetInitialState(
            sm->AddState<ToggleLightGroupState>( 
                true, std::vector<std::string>{ resources->phase12Lighting } 
            )
        );
        addSegmentLogic( sm, {
            LevelSegments::BossRoom_Platforming,
            LevelSegments::BossRoom_Introduction,
            LevelSegments::BossRoom_Phase1,
            LevelSegments::BossRoom_Phase2
        } );
    }

    // Turn phase 1 & 2 lights off
    {
        auto sm = std::make_shared<SegmentLogicStateMachine>( "Phse 1 & 2 Lights Off", this );
        sm->SetInitialState(
            sm->AddState<ToggleLightGroupState>( 
                false, std::vector<std::string>{ resources->phase12Lighting } 
            )
        );
        addSegmentLogic( sm, {
            LevelSegments::BossRoom_Phase3,
            LevelSegments::BossRoom_Phase4,
            LevelSegments::BossRoom_Phase5
        } );
    }

    // Turn phase 3 lights on
    {
        auto sm = std::make_shared<SegmentLogicStateMachine>( "Phase 3 Lights On", this );
        sm->SetInitialState(
            sm->AddState<ToggleLightGroupState>(
                true, std::vector<std::string>{ resources->phase3CenterLights }
            )
        );
        addSegmentLogic( sm, {
            LevelSegments::BossRoom_Phase3
        } );
    }

    // Turn phase 3 lights off
    {
        auto sm = std::make_shared<SegmentLogicStateMachine>( "Phase 3 Lights Off", this );
        sm->SetInitialState(
            sm->AddState<ToggleLightGroupState>(
                false, std::vector<std::string>{ 
                    resources->phase3CenterLights,
                    resources->phase3BossLights,
                    resources->phase3LeftLights,
                    resources->phase3RightLights
                }
            )
        );
        addSegmentLogic( sm, {
            LevelSegments::BossRoom_Platforming,
            LevelSegments::BossRoom_Introduction,
            LevelSegments::BossRoom_Phase1,
            LevelSegments::BossRoom_Phase2
        } );
    }
    {
        auto sm = std::make_shared<SegmentLogicStateMachine>( "Phase 3 Lights Off", this );
        sm->SetInitialState(
            sm->AddState<ToggleLightGroupState>(
                false, std::vector<std::string>{ 
                    resources->phase3CenterLights,
                    resources->phase3BossLights,
                    resources->phase3LeftLights,
                    resources->phase3RightLights
                }
            )
        );
        addSegmentLogic( sm, {
            LevelSegments::BossRoom_Phase4,
            LevelSegments::BossRoom_Phase5
        } );
    }

    // Turn phase 4 lights on
    {
        auto sm = std::make_shared<SegmentLogicStateMachine>( "Phase 4 Lights On", this );
        sm->SetInitialState(
            sm->AddState<ToggleLightGroupState>(
                true, std::vector<std::string>{ resources->phase4Lights }
            )
        );
        addSegmentLogic( sm, {
            LevelSegments::BossRoom_Phase4
        } );
    }

    // Turn phase 4 lights off
    {
        auto sm = std::make_shared<SegmentLogicStateMachine>( "Phase 4 Lights Off", this );
        sm->SetInitialState(
            sm->AddState<ToggleLightGroupState>(
                false, std::vector<std::string>{ 
                    resources->phase4Lights
                }
            )
        );
        addSegmentLogic( sm, {
            LevelSegments::BossRoom_Platforming,
            LevelSegments::BossRoom_Introduction,
            LevelSegments::BossRoom_Phase1,
            LevelSegments::BossRoom_Phase2,
            LevelSegments::BossRoom_Phase3
        } );
    }
    {
        auto sm = std::make_shared<SegmentLogicStateMachine>( "Phase 4 Lights Off", this );
        sm->SetInitialState(
            sm->AddState<ToggleLightGroupState>(
                false, std::vector<std::string>{ 
                    resources->phase4Lights
                }
            )
        );
        addSegmentLogic( sm, {
            LevelSegments::BossRoom_Phase5
        } );
    }

    // Turn phase 5 lights on
    {
        auto sm = std::make_shared<SegmentLogicStateMachine>( "Phase 5 Lights On", this );
        sm->SetInitialState(
            sm->AddState<ToggleLightGroupState>(
                true, std::vector<std::string>{ resources->phase5Lights }
            )
        );
        addSegmentLogic( sm, {
            LevelSegments::BossRoom_Phase5
        } );
    }

    // Turn phase 5 lights off
    {
        auto sm = std::make_shared<SegmentLogicStateMachine>( "Phase 5 Lights Off", this );
        sm->SetInitialState(
            sm->AddState<ToggleLightGroupState>(
                false, std::vector<std::string>{ 
                    resources->phase5Lights
                }
            )
        );
        addSegmentLogic( sm, {
            LevelSegments::BossRoom_Platforming,
            LevelSegments::BossRoom_Introduction,
            LevelSegments::BossRoom_Phase1,
            LevelSegments::BossRoom_Phase2,
            LevelSegments::BossRoom_Phase3,
            LevelSegments::BossRoom_Phase4
        } );
    }

    // Setup logic for the introduction cinematic
    {
        auto cinematicStateM = std::make_shared<SegmentLogicStateMachine>( "Boss Cinematic", this );

        auto createPlayers = cinematicStateM->AddState<SpawnPlayersState>( false, false );
        auto lockPlayers = cinematicStateM->AddState<LockPlayerCharacterControllerState>( true, true, true, true );
        auto unlockPlayers = cinematicStateM->AddState<LockPlayerCharacterControllerState>( false, false, false, false );
        auto repositionAndClose = cinematicStateM->AddState<RepositionPlayersAndCloseDoorState>( );
        auto changeSegment = cinematicStateM->AddState<ChangeSegmentState>( LevelSegments::BossRoom_Phase1 );
        auto turnCinCamOn = cinematicStateM->AddState<ToggleCameraActiveState>( resources->bossIntroCinCameraName, true );
        auto turnCinCamOff = cinematicStateM->AddState<ToggleCameraActiveState>( resources->bossIntroCinCameraName, false );
        auto tweenOut = cinematicStateM->AddState<PlayerViewportTweeningState>( ViewportTweenType::SplitOutRightLeft, true, false );
        auto tweenIn = cinematicStateM->AddState<PlayerViewportTweeningState>( ViewportTweenType::SplitInLeftRight, true, true );
        auto toggleHudOn = cinematicStateM->AddState<ToggleHudState>( true );
        auto toggleHudOff = cinematicStateM->AddState<ToggleHudState>( false );

        createPlayers->AddTransition( lockPlayers, "Lock Dem" );

        lockPlayers->AddTransition( toggleHudOff, "Toggle Hud Off" );

        toggleHudOff->AddTransition( turnCinCamOn, "Turn Cin Camera On" );

        turnCinCamOn->AddTransition( tweenOut, "Tween Out" );

        tweenOut->AddTransition( repositionAndClose, "Reposition Players" )
                ->AddCondition<sm::TimerCondition>( TimeSpan::FromSeconds( 3.0f ) );

        repositionAndClose->AddTransition( changeSegment, "Go To Phase 1" );

        changeSegment->AddTransition( tweenIn, "Tween Viewports In" )
                     ->AddCondition<sm::TimerCondition>( TimeSpan::FromSeconds( 13.0f ) );

        tweenIn->AddTransition( toggleHudOn, "Go To Hud On" );

        toggleHudOn->AddTransition( turnCinCamOff, "Turn Cin Camera Off" );

        turnCinCamOff->AddTransition( unlockPlayers, "Unlock Players" );

        cinematicStateM->SetInitialState( createPlayers );

        addSegmentLogic( cinematicStateM, {
            LevelSegments::BossRoom_Introduction,
            LevelSegments::BossRoom_Phase1
        } );
    }

    // Setup logic for transitioning from phase 1 to phase 2
    {
        auto p1to2StateM = std::make_shared<SegmentLogicStateMachine>( "Boss Phase 2 Transition", this );

        auto createPlayers = p1to2StateM->AddState<SpawnPlayersState>( false, false );
        auto lockPlayers = p1to2StateM->AddState<LockPlayerCharacterControllerState>( true, true, true, true );
        auto unlockPlayers = p1to2StateM->AddState<LockPlayerCharacterControllerState>( false, false, false, false );
        auto toggleHudOn = p1to2StateM->AddState<ToggleHudState>( true );
        auto toggleHudOff = p1to2StateM->AddState<ToggleHudState>( false );
        auto turnCamOn = p1to2StateM->AddState<ToggleCameraActiveState>( resources->bossTransitionCameraName, true );
        auto turnCamOff = p1to2StateM->AddState<ToggleCameraActiveState>( resources->bossTransitionCameraName, false );
        auto tweenOut = p1to2StateM->AddState<PlayerViewportTweeningState>( ViewportTweenType::SplitOutRightLeft, true, false );
        auto tweenIn = p1to2StateM->AddState<PlayerViewportTweeningState>( ViewportTweenType::SplitInLeftRight, true, true );

        createPlayers->AddTransition( lockPlayers, "Lock Players" );
        lockPlayers->AddTransition( toggleHudOff, "Toggle Hud Off" ); 
        toggleHudOff->AddTransition( turnCamOn, "Turn Cam On" );
        turnCamOn->AddTransition( tweenOut, "Tween Out" );
        tweenOut->AddTransition( tweenIn, "Tween In" )
                ->AddCondition<sm::TimerCondition>( TimeSpan::FromSeconds( 3.0f ) );
        tweenIn->AddTransition( toggleHudOn, "Toggle Hud On" );
        toggleHudOn->AddTransition( turnCamOff, "Turn Cam Off" );
        turnCamOff->AddTransition( unlockPlayers, "Unlock Players" );

        p1to2StateM->SetInitialState( createPlayers );

        addSegmentLogic( p1to2StateM, {
            LevelSegments::BossRoom_Phase2
        } );
    }

    // Setup logic for phase3 cinematic
    {
        // Wait for the event to be sent
        // // When sent, Turn on cinematic camera and start the anim, slide viewports out and lock players
        // // on finish, slide viewports back in, unlcok players, send event for phase3 start?

        auto sm = std::make_shared<SegmentLogicStateMachine>( "Phase3 Cinematic", this );

        auto playerWinOn = sm->AddState<PlayerWinState>( true );
        auto playerWinOff = sm->AddState<PlayerWinState>( false );
        auto waitForTrig = sm->AddState<Phase3WaitForTriggerState>( );
        auto turnOnCinematicCam = sm->AddState<ToggleCameraActiveState>( resources->phase3CinematicCamera, true );
        auto turnOffCinematicCam = sm->AddState<ToggleCameraActiveState>( resources->phase3CinematicCamera, false );
        auto playCinematicFocalP = sm->AddState<PlayEntityAnimatorState>( resources->phase3CinematicFocalPoint, false );
        auto playCinematicCam = sm->AddState<PlayEntityAnimatorState>( resources->phase3CinematicCamera, true );
        auto tweenOut = sm->AddState<PlayerViewportTweeningState>( ViewportTweenType::SplitOutRightLeft, true, false );
        auto tweenIn = sm->AddState<PlayerViewportTweeningState>( ViewportTweenType::SplitInLeftRight, true, true );
        auto toggleHudOn = sm->AddState<ToggleHudState>( true );
        auto toggleHudOff = sm->AddState<ToggleHudState>( false );
        auto lockPlayers = sm->AddState<LockPlayerCharacterControllerState>( true, true, true, true );
        auto unlockPlayers = sm->AddState<LockPlayerCharacterControllerState>( false, false, false, false );
        auto turnBossLightOn = sm->AddState<ToggleLightGroupState>( true, std::vector<std::string>{ resources->phase3BossLights } );
        auto turnCenterLightsOff = sm->AddState<ToggleLightGroupState>( false, std::vector<std::string>{ resources->phase3CenterLights } );

        waitForTrig->AddTransition( turnOnCinematicCam, "Turn On Cam" );
        turnOnCinematicCam->AddTransition( lockPlayers, "Lock Players" );
        lockPlayers->AddTransition( toggleHudOff, "Turn Off Hud" );
        toggleHudOff->AddTransition( tweenOut, "Tween Out" );
        tweenOut->AddTransition( playerWinOn, "Win" );
        playerWinOn->AddTransition( playCinematicFocalP, "Play Cinematic" );
        playCinematicFocalP->AddTransition( playCinematicCam, "Play Cinematic Cam" )
                           ->AddCondition<sm::TimerCondition>( TimeSpan::FromSeconds( 7.0f ) );
        playCinematicCam->AddTransition( turnBossLightOn, "Turn On Lights" )
                        ->AddCondition<sm::TimerCondition>( TimeSpan::FromSeconds( 5.0f ) );
        turnBossLightOn->AddTransition( turnCenterLightsOff, "Turn center lights off" );
        turnCenterLightsOff->AddTransition( tweenIn, "Tween Back In" )
                           ->AddCondition<sm::TimerCondition>( TimeSpan::FromSeconds( 4.0f ) );
        tweenIn->AddTransition( playerWinOff, "Win Off" );
        playerWinOff->AddTransition( toggleHudOn, "Hud On" );
        toggleHudOn->AddTransition( turnOffCinematicCam, "Turn Cam Off" );
        turnOffCinematicCam->AddTransition( unlockPlayers, "Unlock Players" );

        sm->SetInitialState( waitForTrig );

        addSegmentLogic(sm, {
            LevelSegments::BossRoom_Phase3
        } );
    }

    // Phase4 get mad cinematic
    {
        auto sm = std::make_shared<SegmentLogicStateMachine>( "Boss Phase 4 Transition", this );

        auto createPlayers = sm->AddState<SpawnPlayersState>( false, false );
        auto lockPlayers = sm->AddState<LockPlayerCharacterControllerState>( true, true, true, true );
        auto unlockPlayers = sm->AddState<LockPlayerCharacterControllerState>( false, false, false, false );
        auto toggleHudOn = sm->AddState<ToggleHudState>( true );
        auto toggleHudOff = sm->AddState<ToggleHudState>( false );
        auto turnCamOn = sm->AddState<ToggleCameraActiveState>( resources->bossTransitionCameraName, true );
        auto turnCamOff = sm->AddState<ToggleCameraActiveState>( resources->bossTransitionCameraName, false );
        auto tweenOut = sm->AddState<PlayerViewportTweeningState>( ViewportTweenType::SplitOutRightLeft, true, false );
        auto tweenIn = sm->AddState<PlayerViewportTweeningState>( ViewportTweenType::SplitInLeftRight, true, true );

        createPlayers->AddTransition( lockPlayers, "Lock Players" );
        lockPlayers->AddTransition( toggleHudOff, "Toggle Hud Off" ); 
        toggleHudOff->AddTransition( turnCamOn, "Turn Cam On" );
        turnCamOn->AddTransition( tweenOut, "Tween Out" );
        tweenOut->AddTransition( tweenIn, "Tween In" )
                ->AddCondition<sm::TimerCondition>( TimeSpan::FromSeconds( 3.0f ) );
        tweenIn->AddTransition( toggleHudOn, "Toggle Hud On" );
        toggleHudOn->AddTransition( turnCamOff, "Turn Cam Off" );
        turnCamOff->AddTransition( unlockPlayers, "Unlock Players" );

        sm->SetInitialState( createPlayers );

        addSegmentLogic(sm, {
            LevelSegments::BossRoom_Phase4
        } );
    }

    // Setup logic for phase5 cinematic logic
    {
        // tween out viewports
        // start animating the camera and focal point
        // tell players to enter the win aniamtion state
        // when camera finishes, tween viewports back in and tell players they aren't winning

        auto sm = std::make_shared<SegmentLogicStateMachine>( "Phase5 Cinematic", this );

        auto repositionPlayers = sm->AddState<SpawnPlayersState>( true, true );
        auto playerWinOn = sm->AddState<PlayerWinState>( true );
        auto playerWinOff = sm->AddState<PlayerWinState>( false );
        auto turnOnCinematicCam = sm->AddState<ToggleCameraActiveState>( resources->phase5CinematicCamera, true );
        auto turnOffCinematicCam = sm->AddState<ToggleCameraActiveState>( resources->phase5CinematicCamera, false );
        auto playCinematicFocalP = sm->AddState<PlayEntityAnimatorState>( resources->phase5CinematicFocalPoint, false );
        auto playCinematicCam = sm->AddState<PlayEntityAnimatorState>( resources->phase5CinematicCamera, false );
        auto turnOffAliveParticles = sm->AddState<ToggleLightGroupState>( false, std::vector<std::string>{ resources->bossAliveParticleGroup } );
        auto turnOnDeadParticles = sm->AddState<ToggleLightGroupState>( true, std::vector<std::string>{ resources->bossDeadParticleGroup } );
        auto tweenOut = sm->AddState<PlayerViewportTweeningState>( ViewportTweenType::SplitOutRightLeft, true, false );
        auto tweenIn = sm->AddState<PlayerViewportTweeningState>( ViewportTweenType::SplitInLeftRight, true, true );
        auto toggleHudOff = sm->AddState<ToggleHudState>( false );
        auto lockPlayers = sm->AddState<LockPlayerCharacterControllerState>( true, true, true, true );
        auto unlockPlayers = sm->AddState<LockPlayerCharacterControllerState>( false, false, false, false );

        toggleHudOff->AddTransition( lockPlayers, "Lock Players" );
        lockPlayers->AddTransition( turnOnCinematicCam, "Turn On Camera" );
        turnOnCinematicCam->AddTransition( playerWinOn, "Player Won" );
        playerWinOn->AddTransition( tweenOut, "Tween viewports out" );
        tweenOut->AddTransition( playCinematicCam, "Play Cinematic" );
        playCinematicCam->AddTransition( turnOffAliveParticles, "Turn off particles" );
        turnOffAliveParticles->AddTransition( playCinematicFocalP, "Play Focal Point" );
        playCinematicFocalP->AddTransition( repositionPlayers, "Tween back in" )
                           ->AddCondition<sm::TimerCondition>( TimeSpan::FromSeconds( 3.0f ) );
        repositionPlayers->AddTransition( turnOnDeadParticles, "Tween back in" )
                         ->AddCondition<sm::TimerCondition>( TimeSpan::FromSeconds( 7.0f ) );
        turnOnDeadParticles->AddTransition( tweenIn, "Tween back in" )
                           ->AddCondition<sm::TimerCondition>( TimeSpan::FromSeconds( 7.0f ) );
        tweenIn->AddTransition( turnOffCinematicCam, "Turn cam off" );
        turnOffCinematicCam->AddTransition( playerWinOff, "Turn Off Win" );
        playerWinOff->AddTransition( unlockPlayers, "Unlock players" );

        sm->SetInitialState( toggleHudOff );

        addSegmentLogic( sm, {
            LevelSegments::BossRoom_Phase5
        } );
    }

    // Setup logic for ending credits
    {
        auto endingCreditsResources = GetOwner( )->GetComponent<EndingCreditsResources>( );

        auto initStateM = std::make_shared<SegmentLogicStateMachine>( "Init Ending Creidts", this );

        // initial state for spawning the level
        auto initState = initStateM->AddState<InitializeSegmentState>(
            endingCreditsResources->GetWorldData( ),
            LevelSegments::Empty
        );

        initStateM->SetInitialState( initState );

        addSegmentLogic( initStateM, {
            LevelSegments::EndingCredits
        } );
    }
}

SegmentLogicStateMachine::Handle LevelSegmentManager::createSegmentLogic(const std::string &name, LevelSegments segment)
{
    auto sm = std::make_shared<SegmentLogicStateMachine>( name, this );

    m_segmentLogic[ segment ].push_back( sm );

    return sm;
}

void LevelSegmentManager::addSegmentLogic(SegmentLogicStateMachine::Handle logic, std::vector<LevelSegments> segments)
{
    for (auto &segment : segments)
        m_segmentLogic[ segment ].push_back( logic );
}

void LevelSegmentManager::onUpdate(EVENT_HANDLER(World))
{
    if (m_segment == LevelSegments::Empty)
        return;

    // Update all state machines
    for (auto &logic : m_segmentLogic[ m_segment ])
    {
        if (m_enableDebugOutput)
            logic->EnableDebugOutput( &std::cout );
        else
            logic->DisableDebugOutput( );

        if (m_enableDebugOutput)
            std::cout << "LevelSegmentManager: Begin Updating \"" << logic->m_name << "\"" << std::endl;

        logic->Update( );

        if (m_enableDebugOutput)
            std::cout << "LevelSegmentManager: End Updating \"" << logic->m_name << "\"" << std::endl;
    }
}
