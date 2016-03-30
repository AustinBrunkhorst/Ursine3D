/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossChangePhaseState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "BossAIState.h"

#include "LevelSegments.h"

class BossChangePhaseState : public BossAIState
{
public:
    BossChangePhaseState(LevelSegments changeToPhase);

    void Enter(BossAIStateMachine *machine) override;

private:
    LevelSegments m_phase;
};
