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

#include "BossVineState.h"
#include "BossVineStateMachine.h"

void BossVineState::OnEnter(ursine::sm::StateMachine* machine)
{
    Enter( reinterpret_cast<BossVineStateMachine*>( machine ) );
}

void BossVineState::OnUpdate(ursine::sm::StateMachine* machine)
{
    Update( reinterpret_cast<BossVineStateMachine*>( machine ) );
}

void BossVineState::OnExit(ursine::sm::StateMachine* machine)
{
    Exit( reinterpret_cast<BossVineStateMachine*>( machine ) );
}
