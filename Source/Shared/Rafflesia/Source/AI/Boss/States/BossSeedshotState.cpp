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
#include "GameEvents.h"

#include <AnimatorComponent.h>
#include <World.h>

using namespace ursine;
using namespace ecs;

BossSeedshotState::BossSeedshotState(void)
    : BossAIState( "Seedshot" )
    , m_timer( 0.0f )
    , m_on( false )
{
}

void BossSeedshotState::Enter(BossAIStateMachine *machine)
{
    auto boss = machine->GetBoss( )->GetOwner( );

    // find the bone target finder component
    auto components = boss->GetComponentsInChildren<FaceBoneTowardsTarget>( );

    for (auto &component : components)
        m_boneTargetComponents.push_back( component );

    auto animator = boss->GetComponentInChildren<Animator>( );

    if (animator)
    {
        animator->SetEnableBoneManipulation( true );
        animator->SetCurrentState( "Seedshot" );
    }
}

void BossSeedshotState::Update(BossAIStateMachine *machine)
{
    auto dt = Application::Instance->GetDeltaTime( );

    m_timer -= dt;

    auto boss = machine->GetBoss( );

    if (m_timer <= 0.0f)
    { 
        auto bossEntity = boss->GetOwner( );

        bossEntity->Dispatch( m_on ? game::FIRE_END : game::FIRE_START, EventArgs::Empty );

        if (m_on)
            m_timer = boss->GetSeedshotCooldown( );
        else
            m_timer = boss->GetSeedshotInterval( );

        m_on = !m_on;

        if (!m_on)
            findTarget( boss );
    }

    // Find our target if we don't have one
    if (!m_target)
        findTarget( boss );

    rotateTowardsTarget( boss );

    if (m_target && m_boneTargetComponents.size( ))
    {
        // face the bone towards the target
        for (auto &component : m_boneTargetComponents)
            component->SetTargetPosition(
                m_target->GetTransform( )->GetWorldPosition( ) + SVec3::UnitY( ) * 5.0f
            );
    }
}

void BossSeedshotState::Exit(BossAIStateMachine *machine)
{
    auto boss = machine->GetBoss( )->GetOwner( );
    auto animator = boss->GetComponentInChildren<Animator>( );

    if (animator)
        animator->SetEnableBoneManipulation( false );

    auto bossEntity = machine->GetBoss( )->GetOwner( );

    bossEntity->Dispatch( game::FIRE_END, EventArgs::Empty );
}

void BossSeedshotState::findTarget(BossAI *boss)
{
    m_target = nullptr;

    auto world = boss->GetOwner( )->GetWorld( );
    auto players = world->GetEntitiesFromFilter( Filter( ).All<PlayerID>( ) );
    float minHealth = std::numeric_limits<float>( ).max( );

    // find the player with the lowest health
    for (auto &player : players)
    {
        auto health = player->GetComponent<Health>( )->GetHealth( );

        if (health <= 0.0f)
            continue;

        if (health < minHealth)
        {
            minHealth = health;
            m_target = player;
        }
    }

    if (!m_target)
        m_target = players[ rand( ) % players.size( ) ];
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
