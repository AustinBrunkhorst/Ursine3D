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

#include "VineAIState.h"
#include "VineAIComponent.h"

class VineWhipState : public VineAIState
{
public:
    VineWhipState(void);

    bool CanExit(void) override { return m_finished; }

    void Enter(VineAIStateMachine *machine) override;
    void Update(VineAIStateMachine *machine) override;
    void Exit(VineAIStateMachine *machine) override;

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

    void playAnimation(ursine::ecs::Animator *animator, const std::string &clip);

    void onAnimationFinished(EVENT_HANDLER(ursine::ecs::Entity));

    void onVineDeath(EVENT_HANDLER(Health));
};
