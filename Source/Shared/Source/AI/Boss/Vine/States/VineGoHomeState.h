/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VineGoHomeState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "VineAIState.h"

class EntityAnimator;

class VineGoHomeState : public VineAIState
{
public:
    VineGoHomeState(void);

    bool CanExit(void) override { return m_finished; }

    void Enter(VineAIStateMachine *machine) override;
    void Update(VineAIStateMachine *machine) override;

private:
    bool m_finished;
    bool m_animating;

    enum class GoHomeState
    {
        Burrowing,
        Digging,
        Uprooting
    };

    GoHomeState m_state;

    void playAnimation(EntityAnimator *animator, const std::string &clip);

    void onAnimationFinished(EVENT_HANDLER(EntityAnimator));
};
