/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BossSeedshotState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "BossAIState.h"

class FaceBoneTowardsTarget;
class BossAI;

class BossSeedshotState : public BossAIState
{
public:
    BossSeedshotState(void);

    void Enter(BossAIStateMachine *machine) override;
    void Update(BossAIStateMachine *machine) override;
    void Exit(BossAIStateMachine *machine) override;

private:

    ursine::ecs::EntityHandle m_target;

    FaceBoneTowardsTarget *m_boneTargetComponent;

    void findTarget(BossAI *boss);
    void rotateTowardsTarget(BossAI *boss);
};
