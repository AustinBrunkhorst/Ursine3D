/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Phase3WaitForTriggerState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "SegmentLogicState.h"

class Phase3WaitForTriggerState : public SegmentLogicState
{
public:

    Phase3WaitForTriggerState(void);

    bool CanExit(void) override { return m_finished; }

    void Enter(SegmentLogicStateMachine *machine) override;

private:

    bool m_finished;

    void onTrigger(EVENT_HANDLER(ursine::ecs::World));

};
