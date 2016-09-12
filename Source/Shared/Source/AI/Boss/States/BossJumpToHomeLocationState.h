/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossJumpToHomeLocation.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "BossAIState.h"

class BossJumpToHomeLocationState : public BossAIState
{
public:
    BossJumpToHomeLocationState(void);

    void Enter(BossAIStateMachine *machine) override;
};
