/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BomberAIComponent.cpp
**
** Author:
** - Joshua Shlemmer- joshua.shlemmer@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BomberAIComponent.h"

//#include <FloatCondition.h>

#include <SystemManager.h>
#include <AI/AIArchetypes/SharedAIStates/WalkState.h>

// use this for anything you want to draw in the editor
//#include <DebugSystem.h>

NATIVE_COMPONENT_DEFINITION( BomberAI );

using namespace ursine;
using namespace ecs;

BomberAI::BomberAI(void)
    :BaseComponent()
{
}

BomberAI::~BomberAI(void)
{
    GetOwner()->GetWorld()->Listener( this )
        .Off( WORLD_UPDATE, &BomberAI::onUpdate );
}

float BomberAI::GetExplosionDelay() const
{
    return m_explosionDelay;
}

void BomberAI::SetExplosionDelay(float delay)
{
    m_explosionDelay = delay;
}

float BomberAI::GetDamage() const
{
    return m_damage;
}

void BomberAI::SetDamage(float dmg)
{
    m_damage = dmg;
}

float BomberAI::GetAttackRange() const
{
    return m_attackRange;
}

void BomberAI::SetAttackRange(float range)
{
    m_attackRange = range;
}

float BomberAI::GetBlastRadius(void) const
{
    return m_blastRadius;
}

void BomberAI::SetBlastRadius(float radius)
{
    m_blastRadius = radius;
}

float BomberAI::GetCohesionScale() const
{
    return m_cohesionScale;
}

void BomberAI::SetCohesionScale(float newScale)
{
    m_cohesionScale = newScale;
}

float BomberAI::GetSeparationScale() const
{
    return m_separationScale;
}

void BomberAI::SetSeparationScale(float newScale)
{
    m_separationScale = newScale;
}

float BomberAI::GetBoidScale() const
{
    return m_boidScale;
}

void BomberAI::SetBoidScale(float newScale)
{
    m_boidScale = newScale;
}

void BomberAI::OnInitialize(void)
{
    GetOwner()->GetWorld()->Listener( this )
        .On( WORLD_UPDATE, &BomberAI::onUpdate );

    // set up the state machine

    GetOwner()->GetWorld()->Listener(this)
        .On(WORLD_UPDATE, &BomberAI::onUpdate);


    // initialize the state machine
    m_stateMachine.Initialize(GetOwner());

    // set up the state machine
    INIT_WALKSTATE("BomberWalkState");

    // rest of state machine goes here
}

void BomberAI::onUpdate(EVENT_HANDLER(World))
{
    // update our state machine
    m_stateMachine.Update();
}
