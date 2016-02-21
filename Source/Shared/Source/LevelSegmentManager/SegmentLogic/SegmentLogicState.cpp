/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SegmentLogicState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "SegmentLogicState.h"
#include "SegmentLogicStateMachine.h"

void SegmentLogicState::OnEnter(ursine::sm::StateMachine* machine)
{
    Enter( reinterpret_cast<SegmentLogicStateMachine*>( machine ) );
}

void SegmentLogicState::OnUpdate(ursine::sm::StateMachine* machine)
{
    Update( reinterpret_cast<SegmentLogicStateMachine*>( machine ) );
}

void SegmentLogicState::OnExit(ursine::sm::StateMachine* machine)
{
    Exit( reinterpret_cast<SegmentLogicStateMachine*>( machine ) );
}
