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
    BossSpawnVinesState(LevelSegments spawnSegment);

    void Enter(BossAIStateMachine *machine) override;

private:

    LevelSegments m_spawnSegment;

};
