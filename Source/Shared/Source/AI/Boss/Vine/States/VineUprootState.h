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

#include "VineAIState.h"

class EntityAnimator;
class VineAI;

class VineUprootState : public VineAIState
{
public:
    VineUprootState(void);

    bool CanExit(void) override { return m_finished; }

    void Enter(VineAIStateMachine *machine) override;
    void Update(VineAIStateMachine *machine) override;
    void Exit(VineAIStateMachine *machine) override;

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

    ursine::SVec3 m_originalDimensions;

    void playAnimation(EntityAnimator *animator, const std::string &clip);

    void onAnimationFinished(EVENT_HANDLER(EntityAnimator));
};
