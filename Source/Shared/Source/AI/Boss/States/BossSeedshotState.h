/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BossSeedshotState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "BossAIState.h"

class BossSeedshotState : public BossAIState
{
public:
    BossSeedshotState(void);

    bool CanExit(void) override { return false; }

    void Update(BossAIStateMachine *machine) override;

private:
    float m_timer;
    bool m_on;
};
