/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimatorControllerState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "AnimatorControllerState.h"
#include "AnimatorControllerStateMachine.h"

void AnimatorControllerState::OnEnter(ursine::sm::StateMachine* machine)
{
    Enter( reinterpret_cast<AnimatorControllerStateMachine*>( machine ) );
}

void AnimatorControllerState::OnUpdate(ursine::sm::StateMachine* machine)
{
    Update( reinterpret_cast<AnimatorControllerStateMachine*>( machine ) );
}

void AnimatorControllerState::OnExit(ursine::sm::StateMachine* machine)
{
    Exit( reinterpret_cast<AnimatorControllerStateMachine*>( machine ) );
}
