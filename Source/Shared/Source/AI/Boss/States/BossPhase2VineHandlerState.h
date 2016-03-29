/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossPhase2VineHandlerState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "BossAIState.h"

#include "VineAIComponent.h"

class BossPhase2VineHandlerState : public BossAIState
{
public:
    BossPhase2VineHandlerState(void);

    void Enter(BossAIStateMachine *machine) override;
    void Update(BossAIStateMachine *machine) override;
    void Exit(BossAIStateMachine *machine) override;

private:

    void onHealthThresholdReached(VineAI *vine);

};
