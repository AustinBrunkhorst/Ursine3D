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

#include <Application.h>

using namespace ursine;

const std::string VineAIStateMachine::WhipCooldown = "WhipCooldown";
const std::string VineAIStateMachine::UprootCooldown = "UprootCooldown";

const std::string VineAIStateMachine::InRange = "InRange";
const std::string VineAIStateMachine::InView = "InView";

const std::string VineAIStateMachine::GoHome = "GoHome";
const std::string VineAIStateMachine::PursueTarget = "PursueTarget";

VineAIStateMachine::VineAIStateMachine(VineAI* ai)
    : m_ai( ai )
{
    AddFloat( WhipCooldown, 0.0f );
    AddFloat( UprootCooldown, 0.0f );
    AddBool( InRange, false );
    AddBool( InView, false );
    AddBool( GoHome, false );
    AddBool( PursueTarget, false );
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
