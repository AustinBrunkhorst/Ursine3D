/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayEntityAnimatorState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "SegmentLogicState.h"

#include "EntityAnimatorComponent.h"

class PlayEntityAnimatorState : public SegmentLogicState
{
public:
    PlayEntityAnimatorState(const std::string &entityName, bool blocking);

    bool CanExit(void) override { return m_finished; }

    void Enter(SegmentLogicStateMachine *machine) override;

private:
    std::string m_entityName;
    bool m_blocking;
    bool m_finished;

    void onAnimationFinished(EVENT_HANDLER(EntityAnimator));
};
