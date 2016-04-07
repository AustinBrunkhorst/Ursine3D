/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayEntityAnimationClipState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "AnimatorControllerState.h"

#include "EntityAnimatorComponent.h"

class PlayEntityAnimationClipState : public AnimatorControllerState
{
public:
    PlayEntityAnimationClipState(
        const std::string &clipName,
        bool blocking = false
    );

    bool CanExit(void) override { return m_finished; }

    void Enter(AnimatorControllerStateMachine *machine) override;

private:
    std::string m_clipName;
    bool m_blocking;
    bool m_finished;

    void onAnimationFinished(EVENT_HANDLER(EntityAnimator));
};
