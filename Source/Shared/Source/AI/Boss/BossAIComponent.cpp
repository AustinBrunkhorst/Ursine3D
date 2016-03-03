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

#include "EntityEvent.h"

NATIVE_COMPONENT_DEFINITION( BossAI );

using namespace ursine;
using namespace ecs;

BossAI::BossAI(void)
    : BaseComponent( )
    , m_segment( LevelSegments::Empty )
    , m_vineCount( 0 )
{
}

const std::string &BossAI::GetSeedshotEntityName(void) const
{
    return m_seedshotEntity;
}

void BossAI::SetSeedshotEntityName(const std::string &entityName)
{
    m_seedshotEntity = entityName;
}

const std::string &BossAI::GetVineArchetype(void) const
{
    return m_vineArchetype;
}

void BossAI::SetVineArchetype(const std::string &vineArchetype)
{
    m_vineArchetype = vineArchetype;
}

Entity *BossAI::GetSeedshotEntity(void)
{
    return GetOwner( )->GetChildByName( m_seedshotEntity );
}

void BossAI::AddSpawnedVine(Entity *vine)
{
    auto health = vine->GetComponent<Health>( );

    health->Listener( this )
        .On( HEALTH_ZERO, &BossAI::onVineDeath );

    ++m_vineCount;
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
    }

    // Boss Phase I
    // - Spawn Vines in the VineSpawnPositions
    // - When all vines die, use seedshot, then sit there and take damage.
    // - If not enough damage is done before a certain amount of time (after vines all die), respwan them
    {
        auto sm = std::make_shared<BossAIStateMachine>( this );

        auto spawnVines = sm->AddState<BossSpawnVinesState>( );
        auto seedShot = sm->AddState<BossSeedshotState>( );

        spawnVines->AddTransition( seedShot, "To Seedshot" );

        sm->SetInitialState( spawnVines );

        m_bossLogic[ 0 ].push_back( sm );
    }
}

void BossAI::onHierachyConstructed(EVENT_HANDLER(Entity))
{
    // get seed shot so we can activate weapon
    // has to be done this way due to inheritance
    game::WeaponActivationEventArgs args( GetOwner( ) );

    GetSeedshotEntity( )->Dispatch( game::ACTIVATE_WEAPON, &args );
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
}
