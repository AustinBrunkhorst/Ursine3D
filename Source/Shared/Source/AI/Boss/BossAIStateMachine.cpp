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

const std::string BossAIStateMachine::VineCount = "Vine Count";

BossAIStateMachine::BossAIStateMachine(BossAI *boss)
    : m_boss( boss )
{
    AddInt( VineCount, 0 );
}

void BossAIStateMachine::Update(void)
{
    StateMachine::Update( );
}

BossAI *BossAIStateMachine::GetBoss(void)
{
    return m_boss;
}
