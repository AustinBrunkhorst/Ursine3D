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

#include "BossVineStateMachine.h"

#include "BossVineAIComponent.h"

#include <Application.h>

using namespace ursine;

BossVineStateMachine::BossVineStateMachine(BossVineAI* ai)
    : m_ai( ai )
{
    AddFloat( "Cooldown", 0.0f );
}

void BossVineStateMachine::Update(void)
{
    auto cooldown = GetFloat( "Cooldown" );

    if (cooldown >= 0.0f)
        cooldown -= Application::Instance->GetDeltaTime( );

    SetFloat( "Cooldown", cooldown );

    StateMachine::Update( );
}

BossVineAI *BossVineStateMachine::GetAI(void)
{
    return m_ai;
}
