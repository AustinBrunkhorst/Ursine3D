/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ToggleHudState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "SegmentLogicState.h"

class ToggleHudState : public SegmentLogicState
{
public:

    ToggleHudState(bool toggle);

    void Enter(SegmentLogicStateMachine *machine) override;

private:

    bool m_toggle;

};
