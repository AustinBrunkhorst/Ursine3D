/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossPhase3RepositionBoss.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "BossAIState.h"

class BossPhase3RepositionBoss : public BossAIState
{
public:
    BossPhase3RepositionBoss(void);

    void Enter(BossAIStateMachine *machine) override;
};
