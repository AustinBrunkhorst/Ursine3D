/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SpawnPlayersState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "SegmentLogicState.h"

class SpawnPlayersState : public SegmentLogicState
{
public:

    SpawnPlayersState(void);

    void Enter(SegmentLogicStateMachine *machine) override;

};
