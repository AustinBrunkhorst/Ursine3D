/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** AgileAIComponent.cpp
**
** Author:
** - Joshua Shlemmer- joshua.shlemmer@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "AgileAIComponent.h"

// condition includes go here
//#include <Utilities/StateMachine/Conditions/BoolCondition.h>
//#include <Utilities/StateMachine/Conditions/FloatCondition.h>

// states go here
#include <AI/AIArchetypes/SharedAIStates/WalkState.h>


// use this for anything you want to draw in the editor
//#include <DebugSystem.h>

NATIVE_COMPONENT_DEFINITION( AgileAI );

using namespace ursine;
using namespace ecs;

AgileAI::AgileAI(void)
    :BaseComponent()
{
}

AgileAI::~AgileAI(void)
{
    GetOwner()->GetWorld()->Listener( this )
        .Off( WORLD_UPDATE, &AgileAI::onUpdate );
}

float AgileAI::GetDamage() const
{
    return m_damage;
}

void AgileAI::SetDamage(float dmg)
{
    m_damage = dmg;
}

float AgileAI::GetAttackRange() const
{
    return m_attackRange;
}

void AgileAI::SetAttackRange(float range)
{
    m_attackRange = range;
}

float AgileAI::GetCohesionScale() const
{
    return m_cohesionScale;
}

void AgileAI::SetCohesionScale(float newScale)
{
    m_cohesionScale = newScale;
}

float AgileAI::GetSeparationScale() const
{
    return m_separationScale;
}

void AgileAI::SetSeparationScale(float newScale)
{
    m_separationScale = newScale;
}

float AgileAI::GetBoidScale() const
{
    return m_boidScale;
}

void AgileAI::SetBoidScale(float newScale)
{
    m_boidScale = newScale;
}

void AgileAI::OnInitialize(void)
{
    GetOwner()->GetWorld()->Listener( this )
        .On( WORLD_UPDATE, &AgileAI::onUpdate );

    // set up the state machine

    GetOwner()->GetWorld()->Listener(this)
        .On(WORLD_UPDATE, &AgileAI::onUpdate);


    // initialize the state machine
    m_stateMachine.Initialize(GetOwner());

    // set up the state machine
    INIT_WALKSTATE("AgileWalk");
}

void AgileAI::onUpdate(EVENT_HANDLER(World))
{
    // update our state machine
    m_stateMachine.Update();
}
