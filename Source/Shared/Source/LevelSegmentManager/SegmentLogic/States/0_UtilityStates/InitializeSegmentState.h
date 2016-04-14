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

class LevelSegmentManager;

class InitializeSegmentState : public SegmentLogicState
{
public:

    InitializeSegmentState(
        const ursine::resources::ResourceReference &loadInWorld,
        LevelSegments unloadSegment,
        bool stopAllSounds = true
    );

    void Enter(SegmentLogicStateMachine *machine) override;

private:

    ursine::resources::ResourceReference m_loadInWorld;
    LevelSegments m_unloadSegment;

    LevelSegmentManager *m_segmentManager;

    bool m_stopAllSounds;

    void onEntityAdded(EVENT_HANDLER(ursine::ecs::World));
};
