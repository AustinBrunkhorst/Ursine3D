/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BossSludgeshotState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "BossAIState.h"

class BossAI;

class BossSludgeshotState : public BossAIState
{
public:
    BossSludgeshotState(float playback = 1.0f);

    bool CanExit(void) override { return m_finished; }

    void Enter(BossAIStateMachine *machine) override;
    void Update(BossAIStateMachine *machine) override;
    void Exit(BossAIStateMachine *machine) override;

private:

    void onAnimationEvent(EVENT_HANDLER(ursine::ecs::Entity));
    void onAnimationFinish(EVENT_HANDLER(ursine::ecs::Entity));

    void shootSludge(void);
    void findTarget(void);
    void rotateTowardsTarget(void);

    BossAI *m_boss;

    ursine::ecs::EntityHandle m_target;

    bool m_finished;

    float m_playback;
};
