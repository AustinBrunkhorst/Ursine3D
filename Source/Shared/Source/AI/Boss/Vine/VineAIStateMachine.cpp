/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BossVineStateMachine.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "VineAIStateMachine.h"

#include "VineAIComponent.h"
#include "HealthComponent.h"

#include <Application.h>

using namespace ursine;

const std::string VineAIStateMachine::WhipCooldown = "WhipCooldown";
const std::string VineAIStateMachine::UprootCooldown = "UprootCooldown";

const std::string VineAIStateMachine::InRange = "InRange";
const std::string VineAIStateMachine::InView = "InView";

const std::string VineAIStateMachine::GoHome = "GoHome";
const std::string VineAIStateMachine::JumpToHome = "JumpToHome";
const std::string VineAIStateMachine::IsHome = "IsHome";

const std::string VineAIStateMachine::PursueTarget = "PursueTarget";

const std::string VineAIStateMachine::Dead = "Dead";

VineAIStateMachine::VineAIStateMachine(VineAI* ai)
    : m_ai( ai )
{
    AddFloat( WhipCooldown, 0.0f );
    AddFloat( UprootCooldown, 0.0f );
    AddBool( InRange, false );
    AddBool( InView, false );
    AddBool( GoHome, false );
    AddBool( JumpToHome, false );
    AddBool( IsHome, true );
    AddBool( PursueTarget, false );
    AddBool( Dead, false );
}

void VineAIStateMachine::Initialize(void)
{
    // subscribe to the health's OnDeath event
    auto health = m_ai->GetOwner( )->GetComponent<Health>( );

    health->Listener( this )
        .On( HEALTH_ZERO, &VineAIStateMachine::onDeath );
}

void VineAIStateMachine::Update(void)
{
    decrementCooldown( WhipCooldown );
    decrementCooldown( UprootCooldown );

    StateMachine::Update( );
}

VineAI *VineAIStateMachine::GetAI(void)
{
    return m_ai;
}

void VineAIStateMachine::decrementCooldown(const std::string &name)
{
    auto cooldown = GetFloat( name );

    if (cooldown >= 0.0f)
        cooldown -= Application::Instance->GetDeltaTime( );

    SetFloat( name, cooldown );
}

void VineAIStateMachine::onDeath(EVENT_HANDLER(Health))
{
    SetBool( Dead, true );
}
