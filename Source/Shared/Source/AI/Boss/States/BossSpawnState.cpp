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

BossSpawnState::BossSpawnState(float playback, bool toIdle)
    : BossAIState( "Boss Spawn" )
    , m_finished( false )
    , m_playback( playback )
    , m_toIdle( toIdle ) { }

void BossSpawnState::Enter(BossAIStateMachine *machine)
{
    auto ai = machine->GetBoss( );
    auto boss = ai->GetOwner( );

    auto animator = boss->GetComponentInChildren<Animator>( );

    if (animator)
    {
        animator->GetOwner( )->Listener( this )
            .On( ENTITY_ANIMATION_FINISH, &BossSpawnState::onAnimationFinished );

        animator->SetCurrentState( "Spike_Up" );
        animator->SetPlaying( true );
        animator->SetTimeScalar( m_playback );

        m_finished = false;
    }
    else
    {
        machine->GetBoss( )->SetUnderground( false );

        m_finished = true;
    }

    ai->SetHomeLocation(
        boss->GetTransform( )->GetWorldPosition( )
    );
}

void BossSpawnState::onAnimationFinished(EVENT_HANDLER(Entity))
{
    EVENT_SENDER(Entity, sender);

    sender->Listener( this )
        .Off( ENTITY_ANIMATION_FINISH, &BossSpawnState::onAnimationFinished );

    auto animator = sender->GetComponent<Animator>( );

    if (animator)
    {
        if (m_toIdle)
            animator->SetCurrentState( "Idle" );

        animator->SetTimeScalar( 1.0f );
    }

    sender->GetComponentInParent<BossAI>( )->SetUnderground( false );

    m_finished = true;
}
