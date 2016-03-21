/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BossSeedshotState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossSeedshotState.h"

#include "BossAIStateMachine.h"
#include "BossAIComponent.h"

#include "FaceBoneTowardsTargetComponent.h"
#include "PlayerIDComponent.h"

#include <AnimatorComponent.h>
#include <World.h>

using namespace ursine;
using namespace ecs;

BossSeedshotState::BossSeedshotState(void)
    : BossAIState( "Seedshot" )
    , m_boneTargetComponent( nullptr )
{
}

void BossSeedshotState::Enter(BossAIStateMachine *machine)
{
    auto boss = machine->GetBoss( )->GetOwner( );

    // find the bone target finder component
    m_boneTargetComponent = boss->GetComponentInChildren<FaceBoneTowardsTarget>( );

    auto animator = boss->GetComponentInChildren<Animator>( );

    if (animator)
        animator->SetEnableBoneManipulation( true );
}

void BossSeedshotState::Update(BossAIStateMachine *machine)
{
    /*auto dt = Application::Instance->GetDeltaTime( );

    m_timer += dt;

    if (m_timer >= 2.0f)
    {
        auto bossEntity = machine->GetBoss( )->GetOwner( );

        if (!m_on)
            bossEntity->Dispatch( game::FIRE_START, EventArgs::Empty );
        else
            bossEntity->Dispatch( game::FIRE_END, EventArgs::Empty );

        m_timer = 0.0f;

        m_on = !m_on;
    }*/

    auto boss = machine->GetBoss( );

    // Find our target if we don't have one
    if (!m_target)
        findTarget( boss );

    rotateTowardsTarget( boss );

    if (m_target && m_boneTargetComponent)
    {
        // face the bone towards the target
        m_boneTargetComponent->SetTargetPosition(
            m_target->GetTransform( )->GetWorldPosition( )
        );
    }
}

void BossSeedshotState::Exit(BossAIStateMachine *machine)
{
    auto boss = machine->GetBoss( )->GetOwner( );
    auto animator = boss->GetComponentInChildren<Animator>( );

    if (animator)
        animator->SetEnableBoneManipulation( false );
}

void BossSeedshotState::findTarget(BossAI *boss)
{
    auto world = boss->GetOwner( )->GetWorld( );

    // for now just pick one by random
    auto players = world->GetEntitiesFromFilter( Filter( ).All<PlayerID>( ) );

    if (players.size( ) > 1)
        m_target = players[ rand( ) % 2 ];
    else
        m_target = players[ 0 ];
}

void BossSeedshotState::rotateTowardsTarget(BossAI *boss)
{
    if (!m_target)
        return;

    auto bossTrans = boss->GetOwner( )->GetTransform( );
    auto targetTrans = m_target->GetTransform( );

    auto targetPos = targetTrans->GetWorldPosition( );
    auto bossPos = bossTrans->GetWorldPosition( );

    targetPos.Y( ) = bossPos.Y( );

    bossTrans->LookAt( targetPos, boss->GetSeedshotTurnSpeed( ) );
}
