/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossSpawnVinesState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "BossAIState.h"

class BossSpawnVinesState : public BossAIState
{
public:
    BossSpawnVinesState(void);

    void Enter(BossAIStateMachine *machine) override;

};
