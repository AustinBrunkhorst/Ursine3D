/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossPhse3VineLoopState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "BossAIState.h"

#include "BossAIComponent.h"
#include "VineAIComponent.h"

class BossPhase3VineLoopState : public BossAIState
{
public:
    BossPhase3VineLoopState(void);

    void Enter(BossAIStateMachine *machine) override;
    void Update(BossAIStateMachine *machine) override;

private:
    void onThresholdHit(VineAI *vine);

    BossAI *m_boss;

};
