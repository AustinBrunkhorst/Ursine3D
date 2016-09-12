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
    BossPhase3RepositionBoss(bool onlyBossSpawn = true, bool goToCenterPlanter = false);

    void Enter(BossAIStateMachine *machine) override;

private:
    bool m_onlyBossSpawn;
    bool m_goToCenterPlanter;
};
