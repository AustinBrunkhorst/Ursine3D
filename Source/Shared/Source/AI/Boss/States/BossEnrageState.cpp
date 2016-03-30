/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossEnrageState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossEnrageState.h"

#include "BossAIStateMachine.h"
#include "BossAIComponent.h"

#include <AnimatorComponent.h>
#include <EntityEvent.h>

using namespace ursine;
using namespace ecs;

BossEnrageState::BossEnrageState(void)
    : BossAIState( "Boss Enrage" )
    , m_finished( false ) { }

void BossEnrageState::Enter(BossAIStateMachine *machine)
{
    auto boss = machine->GetBoss( )->GetOwner( );

    auto animator = boss->GetComponentInChildren<Animator>( );

    if (animator)
    {
        animator->GetOwner( )->Listener( this )
            .On( ENTITY_ANIMATION_FINISH, &BossEnrageState::onAnimationFinished );

        animator->SetCurrentState( "Enrage" );
        animator->SetPlaying( true );

        m_finished = false;
    }
    else
    {
        m_finished = true;
    }
}

void BossEnrageState::onAnimationFinished(EVENT_HANDLER(Entity))
{
    EVENT_SENDER(Entity, sender);

    sender->Listener( this )
        .Off( ENTITY_ANIMATION_FINISH, &BossEnrageState::onAnimationFinished );

    auto animator = sender->GetComponent<Animator>( );

    if (animator)
        animator->SetCurrentState( "Idle" );

    m_finished = true;
}
