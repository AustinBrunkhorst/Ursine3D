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

#include <KeyboardManager.h>

NATIVE_COMPONENT_DEFINITION( LevelSegmentManager );

using namespace ursine;
using namespace ecs;

LevelSegmentManager::LevelSegmentManager(void)
    : BaseComponent( )
    , m_segment( LevelSegments::Empty )
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
}

void LevelSegmentManager::initCombatBowl1Logic(void)
{
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

void LevelSegmentManager::addSegmentLogic(LevelSegments segment, SegmentLogicStateMachine::Handle logic)
{
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
