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
        const ursine::Array<std::string> &loadInArchetypes,
        LevelSegments unloadSegment
    );

    void Enter(SegmentLogicStateMachine *machine) override;

private:

    ursine::Array<std::string> m_loadInArchetypes;
    LevelSegments m_unloadSegment;
};
