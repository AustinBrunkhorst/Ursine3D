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

#include "TutorialResourcesComponent.h"
#include "CombatBowl1ResourcesComponent.h"

#include <TimerCondition.h>

NATIVE_COMPONENT_DEFINITION( LevelSegmentManager );

using namespace ursine;
using namespace ecs;

LevelSegmentManager::LevelSegmentManager(void)
    : BaseComponent( )
    , m_segment( LevelSegments::Empty )
    , m_player1( nullptr )
    , m_player2( nullptr )
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

Entity *LevelSegmentManager::GetPlayer1(void)
{
    return m_player1;
}

Entity *LevelSegmentManager::GetPlayer2(void)
{
    return m_player2;
}

void LevelSegmentManager::SetCurrentSegment(LevelSegments segment)
{
    m_segment = segment;

    LevelSegmentChangeArgs args;

    args.segment = segment;

    Dispatch( LevelSegmentManagerEvents::SegmentChanged, &args );
}

void LevelSegmentManager::OnInitialize(void)
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
        resources->archetypesToLoad, 
        LevelSegments::CB1_SimulationStartCinematic
    );

    // Next state for spawning the players
    auto playerCreateState = stateM->AddState<SpawnPlayersState>( );

    initState->AddTransition( playerCreateState, "Go To Init Players" );

    // Make sure the players have their character controller's locked
    auto lockCCState = stateM->AddState<LockPlayerCharacterControllerState>( true, true, true, true );

    playerCreateState->AddTransition( lockCCState, "Go To Locking Player Controller" );

    // After players are spawned tween their viewports
    auto tweenState = stateM->AddState<PlayerViewportTweeningState>( ViewportTweenType::SplitOutUpDown, true );

    auto timedTrans = lockCCState->AddTransition( tweenState, "Go To Tween Viewports" );

    timedTrans->AddCondition<sm::TimerCondition>( "", TimeSpan::FromSeconds( 7.0f ) );

    // After the viewports tween out change the level segment
    auto changeSegState = stateM->AddState<ChangeSegmentState>( LevelSegments::CB1_SimulationStartCinematic );

    tweenState->AddTransition( changeSegState, "To Combat Bowl 1 Cinematic" );

    stateM->SetInitialState( initState );

    addSegmentLogic( stateM, {
        LevelSegments::Tut_OpeningCinematic,
        LevelSegments::Tut_MovementTutorial,
        LevelSegments::Tut_SoloTriggerTutorial,
        LevelSegments::Tut_WeaponPickupTutorial,
        LevelSegments::Tut_HipFireTutorial,
        LevelSegments::Tut_AimFireTutorial,
        LevelSegments::Tut_AmmoPickupTutorial,
        LevelSegments::Tut_ShootMovingTargetsTutorial,
        LevelSegments::Tut_ReviveTutorial,
        LevelSegments::Tut_SimultaneousTriggerTutorial,
        LevelSegments::Tut_SimulationCreationCinematic
    } );
}

void LevelSegmentManager::initCombatBowl1Logic(void)
{
    auto resources = GetOwner( )->GetComponent<CombatBowl1Resources>( );

    // Create a state machine that initializes the scene
    auto stateM = std::make_shared<SegmentLogicStateMachine>( "Init Combat Bowl", this );

    // Initial state for spawning the level
    auto initState = stateM->AddState<InitializeSegmentState>(
        resources->archetypesToLoad,
        LevelSegments::Empty
    );

    // Next state for spawning the players (reposition them if they are present)
    auto playerCreateState = stateM->AddState<SpawnPlayersState>( true, true );

    initState->AddTransition( playerCreateState, "Go To Init Players" );

    stateM->SetInitialState( initState );

    addSegmentLogic( stateM, {
        LevelSegments::CB1_SimulationStartCinematic,
        LevelSegments::CB1_WeaponSelection,
        LevelSegments::CB1_ActivateSystems1,
        LevelSegments::CB1_Combat1,
        LevelSegments::CB1_Combat2,
        LevelSegments::CB1_Combat3,
        LevelSegments::CB1_Combat4,
        LevelSegments::CB1_OpenConduitRoom
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
        logic->Update( );
}
