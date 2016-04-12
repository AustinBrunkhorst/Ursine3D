/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossAIComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossAIComponent.h"

#include "BossSeedshotState.h"
#include "BossSpawnVinesState.h"
#include "BossPollinateState.h"
#include "BossSludgeshotState.h"
#include "BossSpawnState.h"
#include "BossUndergroundState.h"
#include "BossInvulnerableToggleState.h"
#include "BossDazedState.h"
#include "BossChangePhaseState.h"
#include "BossEnrageState.h"
#include "BossPhase2VineHandlerState.h"
#include "BossPhase3WaitTillTriggerState.h"
#include "BossPhase3RepositionBoss.h"
#include "BossJumpToHomeLocationState.h"
#include "BossPhase3VineLoopState.h"
#include "BossPlayAudioEventState.h"
#include "BossEmptyState.h"
#include "BossDeathState.h"

#include "HealthComponent.h"
#include "GameEvents.h"
#include "IntCondition.h"

#include <EntityEvent.h>
#include <DebugSystem.h>
#include <SystemManager.h>
#include <TimerCondition.h>
#include <BoolCondition.h>
#include <FloatCondition.h>

NATIVE_COMPONENT_DEFINITION( BossAI );

using namespace ursine;
using namespace ecs;
using namespace resources;

namespace
{
    static const int kPhaseNumber = 5;

    int GetSegmentIndex(LevelSegments segment)
    {
        switch (segment)
        {
        case LevelSegments::BossRoom_Phase1:
            return 0;
        case LevelSegments::BossRoom_Phase2:
            return 1;
        case LevelSegments::BossRoom_Phase3:
            return 2;
        case LevelSegments::BossRoom_Phase4:
            return 3;
        case LevelSegments::BossRoom_Phase5:
            return 4;
        }

        return -1;
    }
}

BossAI::BossAI(void)
    : BaseComponent( )
    , EventDispatcher<BossAIEvents>( this )
    , m_turnSpeed( 90.0f )
    , m_seedshotInterval( 2.0f )
    , m_seedshotCooldown( 2.0f )
    , m_sludgeshotAnimationTime( 5.0f )
    , m_maxPollinateSpreadAngle( 30.0f )
    , m_pollinateLocalForward( 0.0f, 0.0f, 1.0f )
    , m_pollinateProjectileCount( 0 )
    , m_vineCount( 0 ) 
    , m_phase1HealthThreshold( 75.0f )
    , m_phase1DazedResetTimer( 5.0f )
    , m_phase2HealthThreshold( 50.0f )
    , m_phase2DazedResetTimer( 5.0f )
    , m_phase3HealthThreshold( 25.0f )
    , m_phase3DazedResetTimer( 5.0f )
    , m_phase3NumSludgeshots( 2 )
    , m_phase3SludgeshotAnimScalar( 1.0f )
    , m_phase4NumSludgeshots( 2 )
    , m_phase4SeedshotDuration( 15.0f )
    , m_segment( LevelSegments::Empty )
    , m_underground( false ) { }

const std::string &BossAI::GetSeedshotEntityName(void) const
{
    return m_seedshotEntity;
}

void BossAI::SetSeedshotEntityName(const std::string &entityName)
{
    m_seedshotEntity = entityName;

    NOTIFY_COMPONENT_CHANGED( "seedshotEntity", m_seedshotEntity );
}

float BossAI::GetSeedshotTurnSpeed(void) const
{
    return m_turnSpeed;
}

void BossAI::SetSeedshotTurnSpeed(float turnSpeed)
{
    m_turnSpeed = turnSpeed;

    NOTIFY_COMPONENT_CHANGED( "seedshotTurnSpeed", m_turnSpeed );
}

float BossAI::GetSeedshotInterval(void) const
{
    return m_seedshotInterval;
}

void BossAI::SetSeedshotInterval(float interval)
{
    m_seedshotInterval = interval;

    NOTIFY_COMPONENT_CHANGED( "seedshotInterval", m_seedshotInterval );
}

float BossAI::GetSeedshotCooldown(void) const
{
    return m_seedshotCooldown;
}

void BossAI::SetSeedshotCooldown(float cooldown)
{
    m_seedshotCooldown = cooldown;

    NOTIFY_COMPONENT_CHANGED( "seedshotCooldown", m_seedshotCooldown );
}

const std::string &BossAI::GetSludgeshotEntityName(void) const
{
    return m_sludgeshotEntity;
}

void BossAI::SetSludgeshotEntityName(const std::string &name)
{
    m_sludgeshotEntity = name;

    NOTIFY_COMPONENT_CHANGED( "sludgeshotEntity", m_sludgeshotEntity );
}

float BossAI::GetSludgeshotAnimationTime(void) const
{
    return m_sludgeshotAnimationTime;
}

void BossAI::SetSludgeshotAnimationTime(float time)
{
    m_sludgeshotAnimationTime = time;

    NOTIFY_COMPONENT_CHANGED( "sludgeshotAnimationTime", m_sludgeshotAnimationTime );
}

const ResourceReference &BossAI::GetSludgeshotArchetype(void) const
{
    return m_sludgeshotArchetype;
}

void BossAI::SetSludgeshotArchetype(const ResourceReference &archetype)
{
    m_sludgeshotArchetype = archetype;

    NOTIFY_COMPONENT_CHANGED( "sludgeshotArchetype", m_sludgeshotArchetype );
}

const std::string &BossAI::GetPollinateEntityName(void) const
{
    return m_pollinateEntity;
}

void BossAI::SetPollinateEntityName(const std::string &entityname)
{
    m_pollinateEntity = entityname;

    NOTIFY_COMPONENT_CHANGED( "pollinateEntity", m_pollinateEntity );
}

const SVec3 &BossAI::GetPollinateLocalForward(void) const
{
    return m_pollinateLocalForward;
}

void BossAI::SetPollinateLocalForward(const SVec3 &localForward)
{
    m_pollinateLocalForward = localForward;

    m_pollinateLocalForward.Normalize( );

    NOTIFY_COMPONENT_CHANGED( "pollinateLocalForward", m_pollinateLocalForward );
}

float BossAI::GetMaxPollinateSpreadAngle(void) const
{
    return m_maxPollinateSpreadAngle;
}

void BossAI::SetMaxPollinateSpreadAngle(float angle)
{
    m_maxPollinateSpreadAngle = angle;

    NOTIFY_COMPONENT_CHANGED( "maxPollinateSpreadAngle", m_maxPollinateSpreadAngle );
}

int BossAI::GetPollinateProjectileCount(void) const
{
    return m_pollinateProjectileCount;
}

void BossAI::SetPollinateprojectileCount(int count)
{
    m_pollinateProjectileCount = count;

    NOTIFY_COMPONENT_CHANGED( "pollinateProjectileCount", m_pollinateProjectileCount );
}

const ResourceReference &BossAI::GetPollinateArchetype(void) const
{
    return m_pollinateArchetype;
}

void BossAI::SetPollinateArchetype(const ResourceReference &pollinateArchetype)
{
    m_pollinateArchetype = pollinateArchetype;

    NOTIFY_COMPONENT_CHANGED( "pollinateArchetype", m_pollinateArchetype );
}

const ResourceReference &BossAI::GetVineArchetype(void) const
{
    return m_vineArchetype;
}

void BossAI::SetVineArchetype(const ResourceReference &vineArchetype)
{
    m_vineArchetype = vineArchetype;

    NOTIFY_COMPONENT_CHANGED( "vineArchetype", m_vineArchetype );
}

float BossAI::GetPhase1HealthTransitionThreshold(void) const
{
    return m_phase1HealthThreshold;
}

void BossAI::SetPhase1HealthTransitionThreshold(float threshold)
{
    m_phase1HealthThreshold = threshold;

    NOTIFY_COMPONENT_CHANGED( "phase1HealthTransitionThreshold", m_phase1HealthThreshold );
}

float BossAI::GetPhase1DazedResetTimer(void) const
{
    return m_phase1DazedResetTimer;
}

void BossAI::SetPhase1DazedResetTimer(float timer)
{
    m_phase1DazedResetTimer = timer;

    NOTIFY_COMPONENT_CHANGED( "phase1DazedResetTimer", m_phase1DazedResetTimer );
}

float BossAI::GetPhase2HealthTransitionThreshold(void) const
{
    return m_phase2HealthThreshold;
}

void BossAI::SetPhase2HealthTransitionThreshold(float threshold)
{
    m_phase2HealthThreshold = threshold;

    NOTIFY_COMPONENT_CHANGED( "phase2HealthTransitionThreshold", m_phase2HealthThreshold );
}

float BossAI::GetPhase2DazedResetTimer(void) const
{
    return m_phase2DazedResetTimer;
}

void BossAI::SetPhase2DazedResetTimer(float timer)
{
    m_phase2DazedResetTimer = timer;

    NOTIFY_COMPONENT_CHANGED( "phase2DazedResetTimer", m_phase2DazedResetTimer );
}

float BossAI::GetPhase3DazedResetTimer(void) const
{
    return m_phase3DazedResetTimer;
}

float BossAI::GetPhase3HealthTransitionThreshold(void) const
{
    return m_phase3HealthThreshold;
}

void BossAI::SetPhase3HealthTransitionThreshold(float threshold)
{
    m_phase3HealthThreshold = threshold;

    NOTIFY_COMPONENT_CHANGED( "phase3HealthTransitionThreshold", m_phase3HealthThreshold );
}

void BossAI::SetPhase3DazedResetTimer(float timer)
{
    m_phase3DazedResetTimer = timer;

    NOTIFY_COMPONENT_CHANGED("phase3DazedResetTimer", m_phase3DazedResetTimer);
}

int BossAI::GetPhase3NumSludgeshots(void) const
{
    return m_phase3NumSludgeshots;
}

void BossAI::SetPhase3NumSludgeshots(int num)
{
    m_phase3NumSludgeshots = num;

    NOTIFY_COMPONENT_CHANGED( "phase3NumSludgeshots", m_phase3NumSludgeshots );
}

float BossAI::GetPhase3SludgeshotAnimScalar(void) const
{
    return m_phase3SludgeshotAnimScalar;
}

void BossAI::SetPhase3SludgeshotAnimScalar(float scalar)
{
    m_phase3SludgeshotAnimScalar = scalar;

    NOTIFY_COMPONENT_CHANGED( "phase3SludgeshotAnimScalar", m_phase3SludgeshotAnimScalar);
}

int BossAI::GetPhase4NumSludgeshots(void) const
{
    return m_phase4NumSludgeshots;
}

void BossAI::SetPhase4NumSludgeshots(int num)
{
    m_phase4NumSludgeshots = num;

    NOTIFY_COMPONENT_CHANGED( "phase4NumSludgeshots", m_phase4NumSludgeshots );
}

float BossAI::GetPhase4SeedshotDuration(void) const
{
    return m_phase4SeedshotDuration;
}

void BossAI::SetPhase4SeedshotDuration(float duration)
{
    m_phase4SeedshotDuration = duration;

    NOTIFY_COMPONENT_CHANGED( "phase4SeedshotDuration", m_phase4SeedshotDuration );
}

const ResourceReference &BossAI::GetIntroScream(void) const
{
    return m_introScream;
}

void BossAI::SetIntroScream(const ResourceReference &audioEvent)
{
    m_introScream = audioEvent;

    NOTIFY_COMPONENT_CHANGED( "introScream", m_introScream );
}

const ResourceReference &BossAI::GetHurtSfx(void) const
{
    return m_hurtSfx;
}

void BossAI::SetHurtSfx(const ResourceReference &hurtSfx)
{
    m_hurtSfx = hurtSfx;

    NOTIFY_COMPONENT_CHANGED( "hurtSfx", m_hurtSfx );
}

EntityHandle BossAI::GetSeedshotEntity(void)
{
    return GetOwner( )->GetChildByName( m_seedshotEntity );
}

EntityHandle BossAI::GetSludgeshotEntity(void)
{
    return GetOwner( )->GetChildByName( m_sludgeshotEntity );
}

EntityHandle BossAI::GetPollinateEntity(void)
{
    return GetOwner( )->GetChildByName( m_pollinateEntity );
}

void BossAI::AddSpawnedVine(EntityHandle vine)
{
    // Add it to our vector
    m_vines.push_back( vine );

    // Subscribe to the death
    auto health = vine->GetComponent<Health>( );

    health->Listener( this )
        .On( HEALTH_ZERO, &BossAI::onVineDeath );

    // Subscribe to it's health threshold events
    auto ai = vine->GetComponent<VineAI>( );

    ai->Listener( this )
        .On( VINE_HEALTH_THRESHOLD_REACHED, &BossAI::onVineHealthThresholdReached );

    // Increment our counter
    ++m_vineCount;

    updateVineCount( );
}

const std::vector<EntityHandle> &BossAI::GetVines(void) const
{
    return m_vines;
}

void BossAI::SetVineHealthThresholdCallback(const std::function<void(VineAI*)> &callback)
{
    m_vineHealthThresholdCallback = callback;
}

void BossAI::SetHomeLocation(const SVec3 &location)
{
    m_homeLocation = location;
}

const SVec3 &BossAI::GetHomeLocation(void) const
{
    return m_homeLocation;
}

void BossAI::JumpToHomeLocation(void)
{
    setJumpToHomeLocationBools( );
}

void BossAI::SetSpawnOrientation(const ursine::SQuat &orientation)
{
    m_spawnOrientation = orientation;
}

const SQuat &BossAI::GetSpawnOrientation(void) const
{
    return m_spawnOrientation;
}

bool BossAI::IsUnderground(void) const
{
    return m_underground;
}

void BossAI::SetUnderground(bool flag)
{
    m_underground = flag;
}

void BossAI::OnInitialize(void)
{
    GetOwner( )->GetWorld( )->Listener( this )
        .On( WORLD_UPDATE, &BossAI::onUpdate );

    GetOwner( )->Listener(this)
        .On( ENTITY_HIERARCHY_SERIALIZED, &BossAI::onHierachyConstructed );

    // Find the level segment manager
    auto levelSegmentManager = GetOwner( )->GetWorld( )->GetEntitiesFromFilter( 
        Filter( ).All<LevelSegmentManager>( ) 
    );

    if (levelSegmentManager.size( ))
    {
        auto lm = levelSegmentManager[ 0 ]->GetComponent<LevelSegmentManager>( );

        lm->Listener( this )
            .On( LevelSegmentManagerEvents::SegmentChanged, &BossAI::onLevelSegmentChanged );

        m_segment = lm->GetCurrentSegment( );
    }

    // Boss Phase 1
    // - Boss uproots
    // - Spawn vines
    // - go into seedshotting
    // - boss becomes exausted after all vines become down, and can be damaged now
    // - After exaustion duration, if he isn't below a certain percent, the steps repeat
    {
        auto sm = std::make_shared<BossAIStateMachine>( this );

        auto playScream = sm->AddState<BossPlayAudioEventState>( m_introScream );
        auto spawnBoss = sm->AddState<BossSpawnState>( 1.0f, true );
        auto spawnVines = sm->AddState<BossSpawnVinesState>( LevelSegments::BossRoom_Phase1, 3.0f );
        auto spawnVines2 = sm->AddState<BossSpawnVinesState>( LevelSegments::BossRoom_Phase1, 0.5f );
        auto seedshot = sm->AddState<BossSeedshotState>( );
        auto invulnerable = sm->AddState<BossInvulnerableToggleState>( true );
        auto invulnerable2 = sm->AddState<BossInvulnerableToggleState>( true );
        auto vulnerable = sm->AddState<BossInvulnerableToggleState>( false );
        auto dazed = sm->AddState<BossDazedState>( );
        auto changePhaseState = sm->AddState<BossChangePhaseState>( LevelSegments::BossRoom_Phase2 );

        playScream->AddTransition( spawnBoss, "Spawn Boss" );
        spawnBoss->AddTransition( spawnVines, "To Spawn Vines" );
        spawnVines->AddTransition( invulnerable, "To Invulnerable" );
        invulnerable->AddTransition( seedshot, "To Seedshot" )
                    ->AddCondition<sm::TimerCondition>( TimeSpan::FromSeconds( 5.0f ) );
        seedshot->AddTransition( vulnerable, "To Vulnerable" )
                ->AddCondition<sm::IntCondition>( 
                    BossAIStateMachine::VineCount, sm::Comparison::Equal, 0 
                );
        vulnerable->AddTransition( dazed, "To Dazed" );

        // Go back to spawning vines if the reset timer is up
        dazed->AddTransition( invulnerable2, "Back To Invulnerable" )
             ->AddCondition<sm::TimerCondition>( TimeSpan::FromSeconds( m_phase1DazedResetTimer ) );

        invulnerable2->AddTransition( spawnVines2, "Spawn Vines Again" );
        spawnVines2->AddTransition( seedshot, "Back To Seedshot" );

        // Go to the next phase when the health is below a certain threashold
        auto health = GetOwner( )->GetComponent<Health>( );
        auto max = health->GetMaxHealth( );
        auto fraction = m_phase1HealthThreshold * 0.01f;
        auto threshold = max * fraction;

        dazed->AddTransition( changePhaseState, "To Phase 2" )
             ->AddCondition<sm::FloatCondition>( 
                 BossAIStateMachine::Health, sm::Comparison::LessThan, threshold 
             );


        sm->SetInitialState( playScream );

        m_bossLogic[ 0 ].push_back( sm );
    }

    // Phase 2
    // Boss angry
    // Sound Queue
    // Spawn all 4 vines
    // Send out 2 towards player
    // Timer exists for uprooting again.
    // If damage threshold done is achieved, go home
    {
        auto sm = std::make_shared<BossAIStateMachine>( this );

        auto enrage = sm->AddState<BossEnrageState>( );
        auto invulnerable = sm->AddState<BossInvulnerableToggleState>( true );
        auto vulnerable = sm->AddState<BossInvulnerableToggleState>( false );
        auto spawnVines = sm->AddState<BossSpawnVinesState>( LevelSegments::BossRoom_Phase2, 1.0f );
        auto seedshot = sm->AddState<BossSeedshotState>( );
        auto dazed = sm->AddState<BossDazedState>( );
        auto changePhaseState = sm->AddState<BossChangePhaseState>( LevelSegments::BossRoom_Phase3 );

        enrage->AddTransition( invulnerable, "To Invulneralbe" );
        invulnerable->AddTransition( spawnVines, "To Spawn Vines" );
        spawnVines->AddTransition( seedshot, "To Seedshot" );
        seedshot->AddTransition( vulnerable, "To Vulnerable" )
                ->AddCondition<sm::IntCondition>( 
                    BossAIStateMachine::VineCount, sm::Comparison::Equal, 0 
                );
        vulnerable->AddTransition( dazed, "To Dazed" );

        // Go back to spawning vines if the reset timer is up
        dazed->AddTransition( invulnerable, "Back To Invulnerable" )
             ->AddCondition<sm::TimerCondition>( TimeSpan::FromSeconds( m_phase2DazedResetTimer ) );

        // Go to the next phase when the health is below a certain threashold
        auto health = GetOwner( )->GetComponent<Health>( );
        auto max = health->GetMaxHealth( );
        auto fraction = m_phase2HealthThreshold * 0.01f;
        auto threshold = max * fraction;

        dazed->AddTransition( changePhaseState, "To Phase 3" )
             ->AddCondition<sm::FloatCondition>( 
                 BossAIStateMachine::Health, sm::Comparison::LessThan, threshold 
             );

        sm->SetInitialState( enrage );

        m_bossLogic[ 1 ].push_back( sm );

        auto vineHandlerSM = std::make_shared<BossAIStateMachine>( this );

        auto vineHandler = vineHandlerSM->AddState<BossPhase2VineHandlerState>( );

        vineHandlerSM->SetInitialState( vineHandler );

        m_bossLogic[ 1 ].push_back( vineHandlerSM );
    }

    // Phase 3
    // - Boss goes under ground
    // - Spot light on the bosses home location
    // - Wait till both players are in the trigger zone
    // - Start spawning vines.  Spawn boss when event is received.
    // - Send event when boss is done spawning for the start of the segment.
    // - Go into a loop for the different shit to happen
    {
        auto sm = std::make_shared<BossAIStateMachine>( this );

        // All I need to do now is just spawn the vines and boss, then transition after a timer,
        // then make the wackamole state for swapping spawn positions, and also have the boss sludge shot and what not.
        auto goUnderground = sm->AddState<BossUndergroundState>( );
        auto waitTillTrigger = sm->AddState<BossPhase3WaitTillTriggerState>( );
        auto spawnVines = sm->AddState<BossSpawnVinesState>( LevelSegments::BossRoom_Phase3, 1.75f );
        auto repositionBoss = sm->AddState<BossPhase3RepositionBoss>( );
        auto blankState = sm->AddState<BossPhase3RepositionBoss>( );
        auto spawnBoss = sm->AddState<BossSpawnState>( 0.5f );
        auto invulnerable = sm->AddState<BossInvulnerableToggleState>( true );
        auto seedshot = sm->AddState<BossSeedshotState>( );
        auto goUnderground2 = sm->AddState<BossUndergroundState>( );
        auto jumpToHome = sm->AddState<BossJumpToHomeLocationState>( );
        auto spawn = sm->AddState<BossSpawnState>( );
        auto goUnderground3 = sm->AddState<BossUndergroundState>( );
        auto repositionBoss2 = sm->AddState<BossPhase3RepositionBoss>( );
        auto spawn2 = sm->AddState<BossSpawnState>( 0.5f );
        auto vulnerable = sm->AddState<BossInvulnerableToggleState>( false );
        auto enraged = sm->AddState<BossEnrageState>( );
        auto sludgeshot = sm->AddState<BossSludgeshotState>( m_phase3NumSludgeshots, m_phase3SludgeshotAnimScalar );
        auto goUnderground4 = sm->AddState<BossUndergroundState>( );
        auto reposition3 = sm->AddState<BossPhase3RepositionBoss>( false );
        auto spawn3 = sm->AddState<BossSpawnState>( 1.0f, false );
        auto repositionCenter = sm->AddState<BossPhase3RepositionBoss>( false, true );
        auto spawnCenter = sm->AddState<BossSpawnState>( );
        auto changePhaseState = sm->AddState<BossChangePhaseState>( LevelSegments::BossRoom_Phase4 );

        goUnderground->AddTransition( waitTillTrigger, "To Waiting For Trigger" );
        waitTillTrigger->AddTransition( repositionBoss, "To Reposition Boss" );
        repositionBoss->AddTransition( spawnVines, "Spawn Vines" )
                      ->AddCondition<sm::TimerCondition>( TimeSpan::FromSeconds( 3.0f ) );
        spawnVines->AddTransition( blankState, "Pause" )
                  ->AddCondition<sm::TimerCondition>( TimeSpan::FromSeconds( 11.0f ) );
        blankState->AddTransition( spawnBoss, "Spawn Boss" );
        spawnBoss->AddTransition( invulnerable, "Invulneralbe" )
                 ->AddCondition<sm::TimerCondition>( TimeSpan::FromSeconds( 2.0f ) );
        invulnerable->AddTransition( seedshot, "Seedshot" )
                    ->AddCondition<sm::TimerCondition>( TimeSpan::FromSeconds( 3.0f ) );
        seedshot->AddTransition( goUnderground2, "Go Underground" )
                ->AddCondition<sm::BoolCondition>( BossAIStateMachine::GoHome, true );
        goUnderground2->AddTransition( jumpToHome, "Jump to new home" );
        jumpToHome->AddTransition( spawn, "Spawn Again" );
        spawn->AddTransition( seedshot, "Attack again" );
        seedshot->AddTransition( vulnerable, "Vulnerable" )
                ->AddCondition<sm::IntCondition>( BossAIStateMachine::VineCount, sm::Comparison::Equal, 0 );
        vulnerable->AddTransition( goUnderground3, "Go Underground" );
        goUnderground3->AddTransition( repositionBoss2, "Reposition" );
        repositionBoss2->AddTransition( spawn2, "Spawn on pedistal" );
        spawn2->AddTransition( enraged, "Enraged that mother fucker" );
        enraged->AddTransition( sludgeshot, "Sludgeshot" );
        sludgeshot->AddTransition( goUnderground4, "Go Underground" );
        goUnderground4->AddTransition( reposition3, "Reposition" );
        reposition3->AddTransition( spawn3, "Spawn again" );
        spawn3->AddTransition( sludgeshot, "Sludgeshot" );
        

        // Go to the center position and become dazed once the bosses health has droped below the threshold
        auto health = GetOwner( )->GetComponent<Health>( );
        auto max = health->GetMaxHealth( );
        auto fraction = m_phase3HealthThreshold * 0.01f;
        auto threshold = max * fraction;

        goUnderground4->AddTransition( repositionCenter, "Back To Center" )
                      ->AddCondition<sm::FloatCondition>(
                            BossAIStateMachine::Health, sm::Comparison::LessThan, threshold 
                        );

        repositionCenter->AddTransition( spawnCenter, "SpawnCenter" );
        spawnCenter->AddTransition( changePhaseState, "Dazed Phase 3" );

        sm->SetInitialState( goUnderground );

        m_bossLogic[ 2 ].push_back( sm );

        auto loop = std::make_shared<BossAIStateMachine>( this );

        loop->SetInitialState(
            loop->AddState<BossPhase3VineLoopState>( )
        );

        m_bossLogic[ 2 ].push_back( loop );
    }

    // Phase 4
    // - Spawn all vines after dazed
    // - Have the boss start sludgeshotting
    // - Then he'll go to seedshot then back again
    // - This continues until all vines are down
    // - Then his shield breaks, and he goes into pollinate
    // - The players must go to the safe zone and burn down the boss
    // - Then once the bosses health is below zero, he goes to phase 5 which is the death state
    {
        auto sm = std::make_shared<BossAIStateMachine>( this );

        auto dazed = sm->AddState<BossDazedState>( );
        auto enrage = sm->AddState<BossEnrageState>( );
        auto invulnerable = sm->AddState<BossInvulnerableToggleState>( true );
        auto vulnerable = sm->AddState<BossInvulnerableToggleState>( false );
        auto spawnVines = sm->AddState<BossSpawnVinesState>( LevelSegments::BossRoom_Phase4, 1.0f );
        auto sludgeshot = sm->AddState<BossSludgeshotState>( m_phase4NumSludgeshots, 1.0f, false );
        auto seedshot = sm->AddState<BossSeedshotState>( );
        auto pollinate = sm->AddState<BossPollinateState>( );
        auto changeTo5 = sm->AddState<BossChangePhaseState>( LevelSegments::BossRoom_Phase5 );

        dazed->AddTransition( enrage, "To Sludgeshot" )
             ->AddCondition<sm::TimerCondition>( TimeSpan::FromSeconds( m_phase3DazedResetTimer ) );

        enrage->AddTransition( invulnerable, "To Invulneralbe" );
        invulnerable->AddTransition( spawnVines, "To Spawn Vines" );
        spawnVines->AddTransition( sludgeshot, "To Seedshot" );
        sludgeshot->AddTransition( seedshot, "To Seedshot" );
        seedshot->AddTransition( sludgeshot, "To Sludgeshot" )
                ->AddCondition<sm::TimerCondition>( TimeSpan::FromSeconds( m_phase4SeedshotDuration ) );

        // When all our vines are down, break our shield and start to pollinate
        seedshot->AddTransition( vulnerable, "Vulnerable" )
                ->AddCondition<sm::IntCondition>( BossAIStateMachine::VineCount, sm::Comparison::Equal, 0 );
        sludgeshot->AddTransition( vulnerable, "Vulnerable" )
                  ->AddCondition<sm::IntCondition>( BossAIStateMachine::VineCount, sm::Comparison::Equal, 0 );

        vulnerable->AddTransition( pollinate, "To Pollinate" );

        pollinate->AddTransition( changeTo5, "Change to 5" )
                 ->AddCondition<sm::FloatCondition>( 
                        BossAIStateMachine::Health, sm::Comparison::Equal, 0.0f
                    );

        sm->SetInitialState( dazed );

        m_bossLogic[ 3 ].push_back( sm );

        // Add the vine handler (the one from phase 2)
        auto vineHandler = std::make_shared<BossAIStateMachine>( this );

        auto loop = vineHandler->AddState<BossPhase2VineHandlerState>( );

        vineHandler->SetInitialState( loop );

        m_bossLogic[ 3 ].push_back( vineHandler );
    }

    // Phase 5
    // - Death state
    {
        auto sm = std::make_shared<BossAIStateMachine>( this );

        auto death = sm->AddState<BossDeathState>( );

        sm->SetInitialState( death );

        m_bossLogic[ 4 ].push_back( sm );
    }
}

void BossAI::onHierachyConstructed(EVENT_HANDLER(Entity))
{
    // get seed shot so we can activate weapon
    // has to be done this way due to inheritance
    game::WeaponActivationEventArgs args( GetOwner( ) );

    auto seedshotEntity = GetSeedshotEntity( );

    if (seedshotEntity)
        seedshotEntity->Dispatch( game::ACTIVATE_WEAPON, &args );

    GetOwner( )->GetComponent<Health>( )->Listener( this )
        .On( HEALTH_DAMAGE_TAKEN, &BossAI::onDamageTaken );
}

void BossAI::onUpdate(EVENT_HANDLER(World))
{
    // update health
    updateHealth( );

    int index = GetSegmentIndex( m_segment );

    if (index == -1)
        return;

    auto stateMachines = m_bossLogic[ index ];

    for (auto &machine : stateMachines)
        machine->Update( );
}

void BossAI::onDamageTaken(EVENT_HANDLER(Health))
{
    GetOwner( )->GetComponent<AudioEmitter>( )->PushEvent( m_hurtSfx );
}

void BossAI::onLevelSegmentChanged(EVENT_HANDLER(LevelSegmentManager))
{
    EVENT_ATTRS(LevelSegmentManager, LevelSegmentChangeArgs);

    if (args->segment == m_segment)
        return;

    // If the previous state is within our range
    if (m_segment <= LevelSegments::BossRoom_Phase5 &&
        m_segment >= LevelSegments::BossRoom_Phase1)
    {
        // exit this state
        auto index = GetSegmentIndex( m_segment );

        for (auto &machine : m_bossLogic[ index ])
            machine->Exit( );
    }

    m_segment = args->segment;
}

void BossAI::onVineDeath(EVENT_HANDLER(Health))
{
    EVENT_SENDER(Health, sender);

    auto owner = sender->GetOwner( );

    // Erase it from our vector of vines
    m_vines.erase(
        std::find( m_vines.begin( ), m_vines.end( ), owner )
    );

    // Unsubscribe from the health threshold events
    auto ai = owner->GetComponent<VineAI>( );

    ai->Listener( this )
        .Off( VINE_HEALTH_THRESHOLD_REACHED, &BossAI::onVineHealthThresholdReached );

    // Decrement our counter
    --m_vineCount;

    updateVineCount( );
}

void BossAI::onVineHealthThresholdReached(EVENT_HANDLER(VineAI))
{
    EVENT_SENDER(VineAI, sender);

    if (m_vineHealthThresholdCallback)
        m_vineHealthThresholdCallback( sender );
}

void BossAI::updateHealth(void)
{
    auto health = GetOwner( )->GetComponent<Health>( );

    auto currentHealth = health->GetHealth( );

    for (int i = 0; i < kPhaseNumber; ++i)
    {
        for (auto &machine : m_bossLogic[ i ])
        {
            machine->SetFloat( BossAIStateMachine::Health,  currentHealth );
        }
    }
}

void BossAI::updateVineCount(void)
{
    for (int i = 0; i < kPhaseNumber; ++i)
    {
        for (auto &machine : m_bossLogic[ i ])
        {
            machine->SetInt( BossAIStateMachine::VineCount, m_vineCount );
        }
    }
}

void BossAI::setJumpToHomeLocationBools(void)
{
    for (int i = 0; i < kPhaseNumber; ++i)
    {
        for (auto &machine : m_bossLogic[ i ])
        {
            machine->SetBool( BossAIStateMachine::GoHome, true );
        }
    }
}

#if defined(URSINE_WITH_EDITOR)

void BossAI::visualizePollinateSpread(void)
{
    static float FocalLength = 25.0f;

    auto world = GetOwner( )->GetWorld( );
    auto drawer = world->GetEntitySystem<DebugSystem>( );

    auto pollinateEntity = GetPollinateEntity( );

    if (!pollinateEntity)
        return;

    auto trans = pollinateEntity->GetTransform( );

    // The position of the bone
    auto position = trans->GetWorldPosition( );

    // The position of the center focal point
    auto focus = trans->ToWorld( m_pollinateLocalForward * FocalLength );

    drawer->DrawCone( position, focus, FocalLength, m_maxPollinateSpreadAngle );
}

#endif
