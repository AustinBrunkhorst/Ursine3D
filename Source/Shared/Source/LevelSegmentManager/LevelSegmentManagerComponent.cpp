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
#include "TriggerWaitState.h"
#include "CurrentSegmentCondition.h"

#include "TutorialResourcesComponent.h"
#include "CombatBowl1ResourcesComponent.h"
#include "BossRoomResourcesComponent.h"

#include <TimerCondition.h>

#include <iostream>

NATIVE_COMPONENT_DEFINITION( LevelSegmentManager );

using namespace ursine;
using namespace ecs;

LevelSegmentManager::LevelSegmentManager(void)
    : BaseComponent( )
    , m_segment( LevelSegments::Empty )
    , m_player1( )
    , m_player2( )
    , m_enableDebugOutput( false )
{
}

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
    m_segment = segment;

    LevelSegmentChangeArgs args;

    args.segment = segment;

    Dispatch( LevelSegmentManagerEvents::SegmentChanged, &args );
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
    // subscribe to update
    GetOwner( )->GetWorld( )->Listener( this )
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
        LevelSegments::CB1_SimulationStartCinematic
    );

    auto playerCreateState = stateM->AddState<SpawnPlayersState>( true, true );
    auto lockCCState = stateM->AddState<LockPlayerCharacterControllerState>( true, true, true, true );
    auto tweenState = stateM->AddState<PlayerViewportTweeningState>( ViewportTweenType::SplitInUpDown, true );
    auto unlockCCState = stateM->AddState<LockPlayerCharacterControllerState>( false, false, false, false );
    auto changeSegState = stateM->AddState<ChangeSegmentState>( LevelSegments::Tut_GateOpens );

    // Next state for spawning the players
    initState->AddTransition( playerCreateState, "Go To Init Players" );

    // Make sure the players have their character controller's locked
    playerCreateState->AddTransition( lockCCState, "Go To Locking Player Controller" );

    // After players are spawned tween their viewports
    auto introCin = lockCCState->AddTransition( tweenState, "Go To Tween Viewports" );

    introCin->AddCondition<sm::TimerCondition>(TimeSpan::FromSeconds(10.0f));

    // Unlock players
    tweenState->AddTransition( unlockCCState, "Go To Unlocking Player Controller" );

    // Halt the state machine until the players hit the first trigger, so that we can then
    // Spawn the movmement tutorial prompts
    //auto waitForTrigger = stateM->AddState<TriggerWaitState>( resources->GetGateTriggerName( ) );

    // After the viewports tween out change the level segment
    unlockCCState->AddTransition( changeSegState, "To Gate Opens" );
    
    stateM->SetInitialState( initState );

    addSegmentLogic( stateM, {
        LevelSegments::Tut_OpeningCinematic,
        LevelSegments::Tut_GateOpens,
        LevelSegments::Tut_MovementTutorial,
        LevelSegments::Tut_JumpTutorial,
        LevelSegments::Tut_WeaponPickupTutorial,
        LevelSegments::Tut_HipFireTutorial,
        LevelSegments::Tut_AimFireTutorial,
        LevelSegments::Tut_AmmoPickupTutorial,
        LevelSegments::Tut_ShootMovingTargetsTutorial,
        LevelSegments::Tut_SoloTriggerTutorial,
        LevelSegments::Tut_ReviveTutorial,
    } );

    auto endingState = std::make_shared<SegmentLogicStateMachine>( "To Combat Bowl", this );

    auto lock = endingState->AddState<LockPlayerCharacterControllerState>( true, false, true, false );
    auto changeSeg = endingState->AddState<ChangeSegmentState>( LevelSegments::CB1_SimulationStartCinematic );
    
    auto trans = lock->AddTransition(
        changeSeg, "Change To Change"
    );

    trans->AddCondition<sm::TimerCondition>( TimeSpan::FromSeconds( 6.0f ) );

    endingState->SetInitialState( lock );

    addSegmentLogic( endingState, {
        LevelSegments::Tut_SimultaneousTriggerTutorial
    } );
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
    auto viewportTween = stateM->AddState<PlayerViewportTweeningState>( ViewportTweenType::SplitInUpDown, true );

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
    auto initStateM = std::make_shared<SegmentLogicStateMachine>( "Init Boss Room", this );

    // initial state for spawning the level
    auto initState = initStateM->AddState<InitializeSegmentState>(
        resources->GetWorldData( ),
        LevelSegments::Empty
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

    // Setup logic for the introduction cinematic
    auto cinematicStateM = std::make_shared<SegmentLogicStateMachine>( "Boss Cinematic", this );

    auto createPlayers = initStateM->AddState<SpawnPlayersState>( true, false );
    auto lockPlayers = cinematicStateM->AddState<LockPlayerCharacterControllerState>( true, true, true, true );
    auto unlockPlayers = cinematicStateM->AddState<LockPlayerCharacterControllerState>( false, false, false, false );
    auto repositionAndClose = cinematicStateM->AddState<RepositionPlayersAndCloseDoorState>( );
    auto tweenOut = cinematicStateM->AddState<PlayerViewportTweeningState>( ViewportTweenType::SplitOutRightLeft, true );
    auto tweenIn = cinematicStateM->AddState<PlayerViewportTweeningState>( ViewportTweenType::SplitInLeftRight, true );

    createPlayers->AddTransition( lockPlayers, "Lock Dem" );

    lockPlayers->AddTransition( tweenOut, "Tween Out" );

    tweenOut->AddTransition( repositionAndClose, "Reposition Players" )
            ->AddCondition<sm::TimerCondition>( TimeSpan::FromSeconds( 3.0f ) );

    repositionAndClose->AddTransition( tweenIn, "Go To Tween In" )
                      ->AddCondition<sm::CurrentSegmentCondition>( this, LevelSegments::BossRoom_Phase1 );

    tweenIn->AddTransition( unlockPlayers, "Unlock Players" );

    cinematicStateM->SetInitialState( createPlayers );
    addSegmentLogic( cinematicStateM, {
        LevelSegments::BossRoom_Introduction,
        LevelSegments::BossRoom_Phase1
    } );
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
