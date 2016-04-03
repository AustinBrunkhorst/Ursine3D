/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossUndergroundState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossUndergroundState.h"

#include "BossAIStateMachine.h"
#include "BossAIComponent.h"

#include <AnimatorComponent.h>
#include <EntityEvent.h>

using namespace ursine;
using namespace ecs;

BossUndergroundState::BossUndergroundState(void)
    : BossAIState( "Boss Spawn" )
    , m_finished( false ) { }

void BossUndergroundState::Enter(BossAIStateMachine *machine)
{
    auto boss = machine->GetBoss( )->GetOwner( );

    auto animator = boss->GetComponentInChildren<Animator>( );

    if (animator)
    {
        animator->GetOwner( )->Listener( this )
            .On( ENTITY_ANIMATION_FINISH, &BossUndergroundState::onAnimationFinished );

        animator->SetCurrentState( "Spike_Down" );
        animator->SetPlaying( true );

        m_finished = false;
    }
    else
    {
        m_finished = true;
    }
}

void BossUndergroundState::onAnimationFinished(EVENT_HANDLER(Entity))
{
    EVENT_SENDER(Entity, sender);

    sender->Listener( this )
        .Off( ENTITY_ANIMATION_FINISH, &BossUndergroundState::onAnimationFinished );

    m_finished = true;
}
