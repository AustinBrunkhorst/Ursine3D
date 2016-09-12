/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** FodderAIComponent.cpp
**
** Author:
** - Joshua Shlemmer- joshua.shlemmer@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "FodderAIComponent.h"

// states we are using for the state machine
#include "WalkState.h"
#include "PauseState.h"


#include <SystemManager.h>
#include <Utilities/StateMachine/Conditions/BoolCondition.h>
#include "States/DamageState.h"
#include <HealthSystem/HealthComponent.h>
#include "CollisionEventArgs.h"
#include "EntityEvent.h"
#include <PlayerLogic/PlayerIDComponent.h>
#include <Utilities/StateMachine/Conditions/FloatCondition.h>

// use this for anything you want to draw in the editor
//#include <DebugSystem.h>

NATIVE_COMPONENT_DEFINITION( FodderAI );

using namespace ursine;
using namespace ecs;

FodderAI::FodderAI(void)
    : BaseComponent()
    , m_stateMachine( )
    , m_pauseTime(1.0f)
    , m_damage(1.0f)
    , m_range(3.0f)
    , m_cohesionScale(0.5f)
    , m_separationScale(0.5f)
    , m_boidBehaviorScale(0.5f)
{
}

FodderAI::~FodderAI(void)
{
    GetOwner()->GetWorld()->Listener(this)
        .Off(WORLD_UPDATE, &FodderAI::onUpdate);

    GetOwner()->GetWorld()->Listener(this)
        .Off(ursine::ecs::ENTITY_COLLISION_PERSISTED, &FodderAI::onCollide);
}

float FodderAI::GetPauseTime() const
{
    return m_pauseTime;
}

void FodderAI::SetPauseTime(float newTime)
{
    m_pauseTime = newTime;
}

float FodderAI::GetDamage() const
{
    return m_damage;
}

void FodderAI::SetDamage(float dmg)
{
    m_damage = dmg;
}

float FodderAI::GetAttackRange() const
{
    return m_range;
}

void FodderAI::SetAttackRange(float newRange)
{
    m_range = newRange;
}

float FodderAI::GetCohesionScale() const
{
    return m_cohesionScale;
}

void FodderAI::SetCohesionScale(float newScale)
{
    m_cohesionScale = newScale;
}

float FodderAI::GetSeparationScale() const
{
    return m_separationScale;
}

void FodderAI::SetSeparationScale(float newScale)
{
    m_separationScale = newScale;
}

//TODO: Think of a better name for this value, Boid is not what this is really
float FodderAI::GetBoidScale() const
{
    return m_boidBehaviorScale;
}

void FodderAI::SetBoidScale(float newScale)
{
    m_boidBehaviorScale = newScale;
}

void FodderAI::OnInitialize(void)
{

    GetOwner()->GetWorld()->Listener(this)
        .On(WORLD_UPDATE, &FodderAI::onUpdate);

    GetOwner()->Listener(this)
        .On(ursine::ecs::ENTITY_COLLISION_PERSISTED, &FodderAI::onCollide);

    // initialize the state machine
    m_stateMachine.Initialize( GetOwner() );

    // set up the state machine
    auto walkState = m_stateMachine.AddState<sm::WalkState>( "FodderWalkState" );

    walkState->SetCohesionScale( m_cohesionScale );

    walkState->SetSeparationScale( m_separationScale );

    walkState->SetBoidbehaviorScale(m_boidBehaviorScale);

    walkState->SetAttackRange(m_range);


    auto pauseState = m_stateMachine.AddState<sm::PauseState>( "FodderPauseState", m_pauseTime );

    auto damageState = m_stateMachine.AddState<sm::DamageState>( "FodderDamageState", m_damage );

    damageState->SetAnimationClip("FodderAttack");

    // next connection is to move from walk state to damage state on collision
    auto trans = walkState->AddTransition( damageState, "WalkStateToDamageState" );
    trans->AddCondition<sm::BoolCondition>( "HitPlayer", true );

    damageState->AddTransition( pauseState, "damageStateToWalkState" );


    trans = pauseState->AddTransition(walkState, "PauseStateToWalkState");
    trans->AddCondition<sm::FloatCondition>(
            "PauseTimer",
            sm::Comparison::LessThan, 0.0f
            );

    m_stateMachine.SetInitialState(walkState);


}

void FodderAI::onUpdate(EVENT_HANDLER(World))
{
    // update our state machine
    m_stateMachine.Update();
}

void FodderAI::onCollide(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED))
{
    EVENT_ATTRS(ursine::ecs::Entity, ursine::physics::CollisionEventArgs);

    auto root = args->otherEntity->GetRoot();

    if (!root->HasComponent<PlayerID>())
        return;

    m_stateMachine.SetBool("HitPlayer", true);

}
