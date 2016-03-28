/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossEnrageState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "BossAIState.h"

class BossEnrageState : public BossAIState
{
public:
    BossEnrageState(void);

    bool CanExit(void) override { return m_finished; }

    void Enter(BossAIStateMachine *machine) override;

private:

    bool m_finished;

    void onAnimationFinished(EVENT_HANDLER(ursine::ecs::Entity));
};
