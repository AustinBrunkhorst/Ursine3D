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

#include "HealthComponent.h"
#include "GameEvents.h"
#include "IntCondition.h"

#include <EntityEvent.h>
#include <DebugSystem.h>
#include <SystemManager.h>

NATIVE_COMPONENT_DEFINITION( BossAI );

using namespace ursine;
using namespace ecs;
using namespace resources;

namespace
{
    const int kPhaseNumber = 5;
}

BossAI::BossAI(void)
    : BaseComponent( )
    , m_segment( LevelSegments::Empty )
    , m_vineCount( 0 )
    , m_turnSpeed( 90.0f )
    , m_maxPollinateSpreadAngle( 30.0f )
    , m_pollinateProjectileCount( 0 )
    , m_pollinateLocalForward( 0.0f, 0.0f, 1.0f )
{
}

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

EntityHandle BossAI::GetSeedshotEntity(void)
{
    return GetOwner( )->GetChildByName( m_seedshotEntity );
}

EntityHandle BossAI::GetPollinateEntity(void)
{
    return GetOwner( )->GetChildByName( m_pollinateEntity );
}

void BossAI::AddSpawnedVine(EntityHandle vine)
{
    auto health = vine->GetComponent<Health>( );

    health->Listener( this )
        .On( HEALTH_ZERO, &BossAI::onVineDeath );

    ++m_vineCount;

    updateVineCount( );
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

    // Boss Phase I
    // - Spawn Vines in the VineSpawnPositions
    // - When all vines die, use seedshot, then sit there and take damage.
    // - If not enough damage is done before a certain amount of time (after vines all die), respwan them
    {
        auto sm = std::make_shared<BossAIStateMachine>( this );

        auto spawnVines = sm->AddState<BossSpawnVinesState>( LevelSegments::BossRoom_Phase1, 4.0f );
        /*auto seedShot = sm->AddState<BossSeedshotState>( );

        auto seedshotTrans = spawnVines->AddTransition( seedShot, "To Seedshot" );

        seedshotTrans->AddCondition<sm::IntCondition>( 
            BossAIStateMachine::VineCount, sm::Comparison::Equal, 0 
        );
        */
        sm->SetInitialState( spawnVines );

        m_bossLogic[ 0 ].push_back( sm );
    }

    // TESTING: seedshot
    {
        auto sm = std::make_shared<BossAIStateMachine>( this );

        auto seedshot = sm->AddState<BossSeedshotState>( );

        sm->SetInitialState( seedshot );

        m_bossLogic[ 1 ].push_back( sm );
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
}

void BossAI::onUpdate(EVENT_HANDLER(World))
{
    int index = -1;

    switch (m_segment)
    {
        case LevelSegments::BossRoom_Phase1:
            index = 0;
            break;
        case LevelSegments::BossRoom_Phase2:
            index = 1;
            break;
        case LevelSegments::BossRoom_Phase3:
            index = 2;
            break;
        case LevelSegments::BossRoom_Phase4:
            index = 3;
            break;
        case LevelSegments::BossRoom_Phase5:
            index = 4;
            break;
    }

    if (index == -1)
        return;

    auto stateMachines = m_bossLogic[ index ];

    for (auto &machine : stateMachines)
        machine->Update( );
}

void BossAI::onLevelSegmentChanged(EVENT_HANDLER(LevelSegmentManager))
{
    EVENT_ATTRS(LevelSegmentManager, LevelSegmentChangeArgs);

    m_segment = args->segment;
}

void BossAI::onVineDeath(EVENT_HANDLER(Health))
{
    --m_vineCount;

    updateVineCount( );
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
