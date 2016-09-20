/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SegmentLogicStateMachine.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <StateMachine.h>

class LevelSegmentManager;

class SegmentLogicStateMachine : public ursine::sm::StateMachine
{
    friend class LevelSegmentManager;

public:
    typedef std::shared_ptr<SegmentLogicStateMachine> Handle;

    SegmentLogicStateMachine(const std::string &name, LevelSegmentManager *segmentManager);

    LevelSegmentManager *GetSegmentManager(void);

private:
    LevelSegmentManager *m_segmentManager;

    std::string m_name;
};
