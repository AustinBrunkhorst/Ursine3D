/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossDazedState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "BossAIState.h"

class BossDazedState : public BossAIState
{
public:
    BossDazedState(void);

    void Enter(BossAIStateMachine *machine) override;
    void Exit(BossAIStateMachine *machine) override;

};
