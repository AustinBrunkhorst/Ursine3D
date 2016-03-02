/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VineUprootState.h
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
class BossVineAI;

class VineUprootState : public BossVineState
{
public:
    VineUprootState(void);

    bool CanExit(void) override { return m_finished; }

    void Enter(BossVineStateMachine *machine) override;
    void Update(BossVineStateMachine *machine) override;
    void Exit(BossVineStateMachine *machine) override;

private:
    bool m_finished;
    bool m_animating;

    enum class UprootState
    {
        Burrowing,
        Digging,
        UprootDelay,
        Uprooting
    };

    UprootState m_state;

    float m_delayTimer;

    void playAnimation(EntityAnimator *animator, const std::string &clip);

    float findYPosition(BossVineAI *ai, const ursine::SVec3 &aiPosition);

    bool closeToTarget(BossVineAI *ai);

    void onAnimationFinished(EVENT_HANDLER(EntityAnimator));
};
