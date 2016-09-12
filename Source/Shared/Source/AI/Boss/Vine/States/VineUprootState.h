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
#include "VineAIComponent.h"

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
        GoingHome,
        UprootDelay,
        Uprooting
    };

    UprootState m_state;

    float m_delayTimer;

    ursine::SVec3 m_originalDimensions;

    float m_goHomeTime;
    float m_goHomeTimer;

    void playAnimation(ursine::ecs::Animator *animator, const std::string &clip);

    void onAnimationFinished(EVENT_HANDLER(ursine::ecs::Entity));

    void onVineDeath(EVENT_HANDLER(ursine::ecs::Entity));

    void correctPosition(ursine::ecs::EntityHandle thisVine, ursine::SVec3 &position);

};
