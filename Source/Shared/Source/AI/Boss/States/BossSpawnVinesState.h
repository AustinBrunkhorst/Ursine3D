/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossSpawnVinesState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "BossAIState.h"

#include "LevelSegments.h"

class BossSpawnVinesState : public BossAIState
{
public:
    BossSpawnVinesState(LevelSegments spawnSegment, float delay = 0.0f);

    bool CanExit(void) override { return m_finished; }

    void Enter(BossAIStateMachine *machine) override;
    void Update(BossAIStateMachine *machine) override;
    void Exit(BossAIStateMachine *machine) override;

private:

    LevelSegments m_spawnSegment;
    float m_delay;
    float m_delayTimer;

    bool m_finished;

    std::vector<ursine::ecs::EntityHandle> m_spawners;
    int m_index;
};
