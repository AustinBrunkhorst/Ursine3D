/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TankAIComponent.cpp
**
** Author:
** - Joshua Shlemmer- joshua.shlemmer@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "TankAIComponent.h"

//#include <FloatCondition.h>

#include <SystemManager.h>
#include <AI/AIArchetypes/SharedAIStates/WalkState.h>
#include <AI/AIArchetypes/SharedAIStates/PauseState.h>
#include <Utilities/StateMachine/Conditions/BoolCondition.h>
#include <Utilities/StateMachine/Conditions/FloatCondition.h>
#include "States/TankSlamState.h"

// use this for anything you want to draw in the editor
//#include <DebugSystem.h>

NATIVE_COMPONENT_DEFINITION( TankAI );

using namespace ursine;
using namespace ecs;

TankAI::TankAI(void)
    :BaseComponent()
{
}

TankAI::~TankAI(void)
{
    GetOwner()->GetWorld()->Listener( this )
        .Off( WORLD_UPDATE, &TankAI::onUpdate );
}

float TankAI::GetSlamDelay() const
{
    return m_slamDelay;
}

void TankAI::SetSlamDelay(float delay)
{
    m_slamDelay = delay;
}

float TankAI::GetDamage() const
{
    return m_damage;
}

void TankAI::SetDamage(float dmg)
{
    m_damage = dmg;
}

float TankAI::GetAttackRange() const
{
    return m_attackRange;
}

void TankAI::SetAttackRange(float range)
{
    m_attackRange = range;
}

float TankAI::GetAttackRadius(void) const
{
    return m_attackRadius;
}

void TankAI::SetAttackRadius(float radius)
{
    m_attackRadius = radius;
}

float TankAI::GetCohesionScale() const
{
    return m_cohesionScale;
}

void TankAI::SetCohesionScale(float newScale)
{
    m_cohesionScale = newScale;
}

float TankAI::GetSeparationScale() const
{
    return m_separationScale;
}

void TankAI::SetSeparationScale(float newScale)
{
    m_separationScale = newScale;
}

float TankAI::GetBoidScale() const
{
    return m_boidScale;
}

void TankAI::SetBoidScale(float newScale)
{
    m_boidScale = newScale;
}

void TankAI::OnInitialize(void)
{
    GetOwner()->GetWorld()->Listener( this )
        .On( WORLD_UPDATE, &TankAI::onUpdate );

    // initialize the state machine
    m_stateMachine.Initialize(GetOwner());

    // set up the state machine
    auto walkState = m_stateMachine.AddState<sm::WalkState>("TankWalkState");

    walkState->SetCohesionScale(m_cohesionScale);

    walkState->SetSeparationScale(m_separationScale);

    walkState->SetBoidbehaviorScale(m_boidScale);

    walkState->SetAttackRange(m_attackRange);

     
    auto pauseState = m_stateMachine.AddState<sm::PauseState>("TankPauseState", m_slamDelay);

    auto slamState = m_stateMachine.AddState<sm::TankSlamState>("TankSlamState", m_damage, m_slamDelay);

    // next connection is to move from walk state to damage state on collision
    auto trans = walkState->AddTransition(slamState, "WalkStateToDamageState");
    trans->AddCondition<sm::BoolCondition>("HitPlayer", true);

    slamState->AddTransition(pauseState, "damageStateToWalkState");

    trans = pauseState->AddTransition(walkState, "PauseStateToWalkState");
    trans->AddCondition<sm::FloatCondition>(
        "PauseTimer",
        sm::Comparison::LessThan, 0.0f
        );

    m_stateMachine.SetInitialState(walkState);
}

void TankAI::onUpdate(EVENT_HANDLER(World))
{
    // update our state machine
    m_stateMachine.Update();
}
