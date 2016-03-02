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
{
}

FodderAI::~FodderAI(void)
{
    GetOwner()->GetWorld()->Listener( this )
        .Off( WORLD_UPDATE, &FodderAI::onUpdate );
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

void FodderAI::OnInitialize(void)
{
    GetOwner()->GetWorld()->Listener( this )
        .On( WORLD_UPDATE, &FodderAI::onUpdate );

    // initialize the stae machine
    m_stateMachine.Initialize( GetOwner() );

    // set up the state machine
    auto walkState = m_stateMachine.AddState<sm::WalkState>( "FodderWalkState" );
    auto pauseState = m_stateMachine.AddState<sm::PauseState>( "FodderPauseState", m_pauseTime );
    auto damageState = m_stateMachine.AddState<sm::DamageState>( "FodderDamageState", m_damage );
    
    // since we don't specify a condition, it will try to transition automatically
    pauseState->AddTransition( walkState, "PauseStateToWalkState" );

    // next connection is to move from walk state to damage state on collision
    auto trans = walkState->AddTransition( damageState, "WalkStateToPauseState" );
    trans->AddCondition<sm::BoolCondition>( "HitPlayer", true );

    damageState->AddTransition( pauseState, "damageStateToPauseState" );

    m_stateMachine.SetInitialState(walkState);
}

void FodderAI::onUpdate(EVENT_HANDLER(World))
{
    // update our state machine
    m_stateMachine.Update();
}
