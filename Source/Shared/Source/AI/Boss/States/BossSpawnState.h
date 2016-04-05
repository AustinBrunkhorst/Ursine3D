/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossSpawnState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "BossAIState.h"

class BossSpawnState : public BossAIState
{
public:
    BossSpawnState(float playback = 1.0f);

    bool CanExit(void) override { return m_finished; }

    void Enter(BossAIStateMachine *machine) override;

private:

    bool m_finished;
    float m_playback;

    void onAnimationFinished(EVENT_HANDLER(ursine::ecs::Entity));
};
