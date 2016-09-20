/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ChangeSegmentState.h
**
** Author:
** - JordanEllis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "SegmentLogicState.h"

#include "LevelSegments.h"

class ChangeSegmentState : public SegmentLogicState
{
public:

    ChangeSegmentState(LevelSegments changeToSegment);

    void Enter(SegmentLogicStateMachine *machine) override;

private:

    LevelSegments m_segment;
};
