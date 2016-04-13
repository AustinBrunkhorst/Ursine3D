/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VineDeathState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "VineDeathState.h"

#include "VineAIStateMachine.h"
#include "VineAIComponent.h"

#include <SphereColliderComponent.h>
#include <GhostComponent.h>
#include <EntityEvent.h>

using namespace ursine;
using namespace ecs;

VineDeathState::VineDeathState(void)
    : VineAIState( "Death" )
{
}

void VineDeathState::Enter(VineAIStateMachine *machine)
{
    auto ai = machine->GetAI( );

    m_animator = ai->GetAnimator( );
    m_owner = ai->GetOwner( );

    m_animator->SetTimeScalar( 1.0f );

    // Flop over
    m_animator->SetCurrentState( "Swipe_In" );

    m_animator->GetOwner( )->Listener( this )
        .On( ENTITY_ANIMATION_FINISH, &VineDeathState::onFlopFinish );

    // Delete all colliders
    auto colliders = m_owner->GetComponentsInChildren<SphereCollider>( );

    for (auto &collider : colliders)
        collider->GetOwner( )->RemoveComponent<SphereCollider>( );

    // Delete all ghost bodies
    auto ghosts = m_owner->GetComponentsInChildren<Ghost>( );

    for (auto &ghost : ghosts)
        ghost->GetOwner( )->RemoveComponent<Ghost>( );
}

void VineDeathState::onFlopFinish(EVENT_HANDLER(Entity))
{
    m_animator->GetOwner( )->Listener( this )
        .Off( ENTITY_ANIMATION_FINISH, &VineDeathState::onFlopFinish );

    m_animator->GetOwner( )->Listener( this )
        .On( ENTITY_ANIMATION_FINISH, &VineDeathState::onDigFinish );

    m_animator->SetCurrentState( "Spike_Down" );
}

void VineDeathState::onDigFinish(EVENT_HANDLER(Entity))
{
    m_animator->GetOwner( )->Listener( this )
        .Off( ENTITY_ANIMATION_FINISH, &VineDeathState::onDigFinish );

    m_owner->Delete( );
}
