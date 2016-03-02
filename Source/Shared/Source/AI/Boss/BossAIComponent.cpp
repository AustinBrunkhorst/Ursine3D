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

#include "EntityEvent.h"

#include "GameEvents.h"

NATIVE_COMPONENT_DEFINITION( BossAI );

using namespace ursine;
using namespace ecs;

BossAI::BossAI(void)
    : BaseComponent( )
    , m_stateMachine( this )
    , m_seedshotEntity( "" )
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

Entity *BossAI::GetSeedshotEntity(void)
{
    return GetOwner( )->GetChildByName( m_seedshotEntity );
}

void BossAI::OnInitialize(void)
{
    GetOwner( )->GetWorld( )->Listener( this )
        .On( WORLD_UPDATE, &BossAI::onUpdate );

    GetOwner( )->Listener(this)
        .On( ENTITY_HIERARCHY_SERIALIZED, &BossAI::onHierachyConstructed );

    // Boss Phase I
    // - Spawn Vines in the VineSpawnPositions
    // - When all vines die, use seedshot, then sit there and take damage.
    // - If not enough damage is done before a certain amount of time (after vines all die), respwan them

    // TESTING
    auto seedshot = m_stateMachine.AddState<BossSeedshotState>( );

    m_stateMachine.SetInitialState( seedshot );
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
    m_stateMachine.Update( );
}
