/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossDeathState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "BossAIState.h"

class BossDeathState : public BossAIState
{
public:
    BossDeathState(void);

    void Enter(BossAIStateMachine *machine) override;
    void Update(BossAIStateMachine *machine) override;

};
