/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BossPhase3WaitTillTriggerState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "BossAIState.h"

class BossPhase3WaitTillTriggerState : public BossAIState
{
public:
    BossPhase3WaitTillTriggerState(void);

    bool CanExit(void) override { return m_finished; }

    void Enter(BossAIStateMachine *machine) override;

private:

    bool m_finished;

    void onTrigger(EVENT_HANDLER(ursine::ecs::World));
};
