/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerWinState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "SegmentLogicState.h"

class PlayerWinState : public SegmentLogicState
{
public:
    PlayerWinState(bool toggle);

    void Enter(SegmentLogicStateMachine *machine) override;

private:
    bool m_toggle;
};
