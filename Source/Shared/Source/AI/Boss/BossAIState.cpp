/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BossVineState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossAIState.h"
#include "BossAIStateMachine.h"

void BossAIState::OnEnter(ursine::sm::StateMachine* machine)
{
    Enter( reinterpret_cast<BossAIStateMachine*>( machine ) );
}

void BossAIState::OnUpdate(ursine::sm::StateMachine* machine)
{
    Update( reinterpret_cast<BossAIStateMachine*>( machine ) );
}

void BossAIState::OnExit(ursine::sm::StateMachine* machine)
{
    Exit( reinterpret_cast<BossAIStateMachine*>( machine ) );
}
