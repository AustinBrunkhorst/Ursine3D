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

#include "VineAIState.h"
#include "VineAIStateMachine.h"

void VineAIState::OnEnter(ursine::sm::StateMachine* machine)
{
    Enter( reinterpret_cast<VineAIStateMachine*>( machine ) );
}

void VineAIState::OnUpdate(ursine::sm::StateMachine* machine)
{
    Update( reinterpret_cast<VineAIStateMachine*>( machine ) );
}

void VineAIState::OnExit(ursine::sm::StateMachine* machine)
{
    Exit( reinterpret_cast<VineAIStateMachine*>( machine ) );
}
