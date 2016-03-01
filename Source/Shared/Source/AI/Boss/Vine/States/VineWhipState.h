/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VineWhipState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "BossVineState.h"

class EntityAnimator;

class VineWhipState : public BossVineState
{
public:
    VineWhipState(void);

    bool CanExit(void) override { return m_finished; }

    void Enter(BossVineStateMachine *machine) override;
    void Update(BossVineStateMachine *machine) override;
    void Exit(BossVineStateMachine *machine) override;

private:
    bool m_finished;
    bool m_animating;

    enum class WhipState
    {
        In,
        Rotate,
        Out
    };

    WhipState m_state;

    ursine::SQuat m_middleOrientation;
    ursine::SQuat m_beginOrientation;
    ursine::SQuat m_endOrientation;
    float m_swingTimer;

    void playAnimation(EntityAnimator *animator, const std::string &clip);

    void onAnimationFinished(EVENT_HANDLER(EntityAnimator));
};
