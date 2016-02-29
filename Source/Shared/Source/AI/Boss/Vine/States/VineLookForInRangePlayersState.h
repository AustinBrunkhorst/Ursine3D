/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VineLookForInRangePlayersState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "BossVineState.h"

class VineLookForInRanagePlayersState : public BossVineState
{
public:
    VineLookForInRanagePlayersState(void);

    bool CanExit(void) override { return m_inRange; }

    void Update(BossVineStateMachine *machine) override;

private:
    bool m_inRange;
};
