/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** InitializeSegmentState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "SegmentLogicState.h"

#include "LevelSegments.h"

class InitializeSegmentState : public SegmentLogicState
{
public:

    InitializeSegmentState(
        const std::string &loadInWorld,
        LevelSegments unloadSegment
    );

    void Enter(SegmentLogicStateMachine *machine) override;

private:

    std::string m_loadInWorld;
    LevelSegments m_unloadSegment;
};
