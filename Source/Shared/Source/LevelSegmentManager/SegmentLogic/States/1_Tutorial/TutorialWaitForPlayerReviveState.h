/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TutorialWaitForPlayerReviveState.h
**
** Author:
** - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "SegmentLogicState.h"

class TutorialWaitForPlayerReviveState : public SegmentLogicState
{
public:

    TutorialWaitForPlayerReviveState(void);

    void Enter(SegmentLogicStateMachine *machine) override;

    bool CanExit(void) override { return m_completed; }

private:

    bool m_completed;
    SegmentLogicStateMachine *m_machine;

    void onPlayerRevive(EVENT_HANDLER(ursine::ecs::Entity));
};
