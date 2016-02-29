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

BossVineStateMachine::BossVineStateMachine(BossVineAI* ai)
    : m_ai( ai )
{
}

BossVineAI *BossVineStateMachine::GetAI(void)
{
    return m_ai;
}
