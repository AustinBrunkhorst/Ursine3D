/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossRoomTopAnimationState.cpp
**
** Author:
** - JordanEllis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossRoomTopAnimationState.h"

#include "SegmentLogicStateIncludes.h"
#include "EntityAnimatorComponent.h"
#include "BossRoomResourcesComponent.h"

using namespace ursine;
using namespace ecs;

BossRoomTopAnimationState::BossRoomTopAnimationState(void)
    : SegmentLogicState( "Boss Room Top Animation State" )
    , m_complete( false )
{
}

void BossRoomTopAnimationState::Enter(SegmentLogicStateMachine *machine)
{
    auto segmentManager = machine->GetSegmentManager( )->GetOwner( );
    auto world = segmentManager->GetWorld( );
    auto bossRoom = segmentManager->GetComponent<BossRoomResources>( );

    auto top = world->GetEntityFromName( bossRoom->bossTopEntityName );

    UAssert( top, "Error: Where's the top?" );

    auto anim = top->GetComponent<EntityAnimator>( );

    anim->Listener( this )
        .On( EntityAnimatorEvent::FinishedAnimating, &BossRoomTopAnimationState::onAnimationFinish );

    anim->Play( "Open" );

    m_animator = anim;
}

void BossRoomTopAnimationState::onAnimationFinish(EVENT_HANDLER(EntityAnimator))
{
    EVENT_ATTRS( EntityAnimator, EventArgs );

    m_animator->Play( "Rotate" );
    m_animator->SetLoopAnimation( true );

    sender->Listener( this )
        .Off( EntityAnimatorEvent::FinishedAnimating, &BossRoomTopAnimationState::onAnimationFinish );

    m_complete = true;
}
