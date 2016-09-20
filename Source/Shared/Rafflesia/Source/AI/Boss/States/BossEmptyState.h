/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BossEmptyState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "BossAIState.h"

class BossEmptyState : public BossAIState
{
public:
    BossEmptyState(void) : BossAIState( "Pause" ) { }

    void Enter(BossAIStateMachine *machine) override { }
    void Update(BossAIStateMachine *machine) override { }
    void Exit(BossAIStateMachine *machine) override { }
};
