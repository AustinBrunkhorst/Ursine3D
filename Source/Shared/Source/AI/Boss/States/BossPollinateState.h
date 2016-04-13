/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BossPollinateState.h
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

class BossPollinateState : public BossAIState
{
public:
    BossPollinateState(void);

    void Enter(BossAIStateMachine *machine) override;
    void Update(BossAIStateMachine *machine) override;
    void Exit(BossAIStateMachine *machine) override;

private:

    void onAnimationEvent(EVENT_HANDLER(ursine::ecs::Entity));

    void spewPollin(void);

    void tryDealDamage(ursine::ecs::EntityHandle entity);

    BossAI *m_boss;

    ursine::ecs::EntityHandle m_safetyShield;

    std::unordered_map<ursine::ecs::EntityHandle, float> m_damageMap;
};
