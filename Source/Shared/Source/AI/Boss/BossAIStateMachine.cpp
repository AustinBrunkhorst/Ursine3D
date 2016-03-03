/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BossAIStateMachine.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossAIStateMachine.h"

#include <Application.h>

using namespace ursine;

BossAIStateMachine::BossAIStateMachine(BossAI *boss)
    : m_boss( boss )
{
    AddFloat( "Cooldown", 0.0f );
}

void BossAIStateMachine::Update(void)
{
    auto cooldown = GetFloat( "Cooldown" );

    if (cooldown >= 0.0f)
        cooldown -= Application::Instance->GetDeltaTime( );

    SetFloat( "Cooldown", cooldown );

    StateMachine::Update( );
}

BossAI *BossAIStateMachine::GetBoss(void)
{
    return m_boss;
}
