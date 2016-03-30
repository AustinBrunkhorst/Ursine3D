/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** NukerAIComponent.cpp
**
** Author:
** - Joshua Shlemmer- joshua.shlemmer@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "NukerAIComponent.h"

#include <SystemManager.h>
#include <AI/AIArchetypes/SharedAIStates/WalkState.h>

// use this for anything you want to draw in the editor
//#include <DebugSystem.h>

NATIVE_COMPONENT_DEFINITION( NukerAI );

using namespace ursine;
using namespace ecs;

NukerAI::NukerAI(void)
    :BaseComponent()
{
}

NukerAI::~NukerAI(void)
{
    GetOwner()->GetWorld()->Listener( this )
        .Off( WORLD_UPDATE, &NukerAI::onUpdate );
}

float NukerAI::GetDamage() const
{
    return m_damage;
}

void NukerAI::SetDamage(float dmg)
{
    m_damage = dmg;
}

float NukerAI::GetAttackRange() const
{
    return m_attackRange;
}

void NukerAI::SetAttackRange(float range)
{
    m_attackRange = range;
}

float NukerAI::GetCohesionScale() const
{
    return m_cohesionScale;
}

void NukerAI::SetCohesionScale(float newScale)
{
    m_cohesionScale = newScale;
}

float NukerAI::GetSeparationScale() const
{
    return m_separationScale;
}

void NukerAI::SetSeparationScale(float newScale)
{
    m_separationScale = newScale;
}

float NukerAI::GetBoidScale() const
{
    return m_boidScale;
}

void NukerAI::SetBoidScale(float newScale)
{
    m_boidScale = newScale;
}

void NukerAI::OnInitialize(void)
{
    GetOwner()->GetWorld()->Listener( this )
        .On( WORLD_UPDATE, &NukerAI::onUpdate );

    // set up the state machine

    // initialize the state machine
    m_stateMachine.Initialize(GetOwner());

    // set up the state machine
    INIT_WALKSTATE("NukerAIWalk");
}

void NukerAI::onUpdate(EVENT_HANDLER(World))
{
    // update our state machine
    m_stateMachine.Update();
}
