/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossSpawnState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossSpawnState.h"

#include "BossAIStateMachine.h"
#include "BossAIComponent.h"

#include <AnimatorComponent.h>
#include <EntityEvent.h>

using namespace ursine;
using namespace ecs;

BossSpawnState::BossSpawnState(void)
    : BossAIState( "Boss Spawn" )
    , m_finished( false ) { }

void BossSpawnState::Enter(BossAIStateMachine *machine)
{
    auto boss = machine->GetBoss( )->GetOwner( );

    auto animator = boss->GetComponentInChildren<Animator>( );

    if (animator)
    {
        animator->GetOwner( )->Listener( this )
            .On( ENTITY_ANIMATION_FINISH, &BossSpawnState::onAnimationFinished );

        animator->SetCurrentState( "Spike_Up" );
        animator->SetPlaying( true );

        m_finished = false;
    }
    else
    {
        m_finished = true;
    }
}

void BossSpawnState::onAnimationFinished(EVENT_HANDLER(Entity))
{
    EVENT_SENDER(Entity, sender);

    sender->Listener( this )
        .Off( ENTITY_ANIMATION_FINISH, &BossSpawnState::onAnimationFinished );

    auto animator = sender->GetComponent<Animator>( );

    if (animator)
        animator->SetCurrentState( "Idle" );

    m_finished = true;
}
