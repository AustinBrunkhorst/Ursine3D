/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossHealthCorrectionState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "BossAIState.h"

class BossHealthCorrectionState : public BossAIState
{
public:
    BossHealthCorrectionState(float fraction);

    void Enter(BossAIStateMachine *machine) override;

private:
    float m_fraction;
};
