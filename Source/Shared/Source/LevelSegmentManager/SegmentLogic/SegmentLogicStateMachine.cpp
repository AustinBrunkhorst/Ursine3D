/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SegmentLogicStateMachine.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "SegmentLogicStateMachine.h"

SegmentLogicStateMachine::SegmentLogicStateMachine(const std::string &name, LevelSegmentManager *segmentManager)
    : m_segmentManager( segmentManager )
    , m_name( name )
{
}

LevelSegmentManager *SegmentLogicStateMachine::GetSegmentManager(void)
{
    return m_segmentManager;
}
